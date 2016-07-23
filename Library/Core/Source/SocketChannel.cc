// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <Core/Config.h>

#if RL_CHANNELS == RL_CHANNELS_SOCKET

#include <Core/Message.h>
#include <Core/SharedMemory.h>
#include <Core/Utilities.h>

#include "SocketChannel.h"

#include <errno.h>
#include <fcntl.h>
#include <poll.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include <mutex>

namespace rl {
namespace core {

/*
 *  Common constants.
 */
static const size_t kMaxInlineBufferSize = 4096;
static const size_t kMaxControlBufferItemCount = 24;
static const size_t kMaxControlBufferSize =
    CMSG_SPACE(sizeof(SocketPair::Handle) * kMaxControlBufferItemCount);

/**
 *  This header is always the first item present in the scatter gather array
 *  used in all socket payloads.
 */
class SocketPayloadHeader {
 public:
  SocketPayloadHeader() : SocketPayloadHeader(false, 0) {}

  SocketPayloadHeader(bool isDataInline, uint8_t oolDescriptors)
      : _oolDescriptors(oolDescriptors), _isDataInline(isDataInline) {}

  uint8_t oolDescriptors() const { return _oolDescriptors; }

  bool isDataInline() const { return _isDataInline; }

 private:
  uint8_t _oolDescriptors;
  bool _isDataInline;

  RL_DISALLOW_COPY_AND_ASSIGN(SocketPayloadHeader);
};

static_assert(rl_trivially_copyable(SocketPayloadHeader),
              "The socket payload must be trivially copyable");

SocketChannel::SocketChannel(Channel& channel)
    : _channel(channel),
      _pair(std::make_shared<SocketPair>(kMaxInlineBufferSize)) {
  setup();
}

SocketChannel::SocketChannel(Channel& channel, RawAttachment attachment)
    : _channel(channel),
      _pair(std::make_shared<SocketPair>(std::move(attachment),
                                         kMaxInlineBufferSize)) {
  setup();
}

void SocketChannel::setup() {
  /*
   *  Setup the channel message buffers.
   */
  _inlineMessageBuffer.resize(kMaxInlineBufferSize);
  _controlBuffer.resize(kMaxControlBufferSize);
}

SocketChannel::~SocketChannel() = default;

std::shared_ptr<EventLoopSource> SocketChannel::createSource() const {
  EventLoopSource::RWHandlesProvider provider = [&]() {
    return EventLoopSource::Handles(_pair->readHandle(), _pair->writeHandle());
  };

  EventLoopSource::IOHandler readHandler = [this](EventLoopSource::Handle) {
    return _channel.readPendingMessageNow();
  };

  /*
   *  We are specifying a null write handler since we will
   *  never directly signal this source. Instead, we will write
   *  to the handle directly.
   *
   *  The channel owns the socket handle, so there is no deallocation
   *  callback either.
   */
  return std::make_shared<EventLoopSource>(provider, nullptr, readHandler,
                                           nullptr, nullptr);
}

bool SocketChannel::doTerminate() {
  return true;
}

IOResult SocketChannel::writeMessages(Messages&& messages,
                                      ClockDurationNano timeout) {
  for (const auto& message : messages) {
    /*
     *  TODO: All messages need to be written in one shot so that timeout
     *        guarantees hold.
     */
    auto result = writeMessageSingle(message, timeout);
    if (result != IOResult::Success) {
      return result;
    }
  }

  return IOResult::Success;
}

static IOResult SocketSendMessage(SocketPair::Handle writer,
                                  struct msghdr* messageHeader,
                                  size_t expectedSendSize,
                                  ClockDurationNano timeout) {
  int64_t sent = 0;
  while (true) {
    sent = RL_TEMP_FAILURE_RETRY(::sendmsg(writer, messageHeader, 0));

    if (sent == -1 && errno == EAGAIN) {
      if (timeout.count() == 0) {
        /*
         *  No need for an extra syscall if the timeout is zero.
         */
        return IOResult::Timeout;
      }

      /*
       *  We definitely need to poll for write, setup the poll structure
       */
      struct pollfd pollFd = {
          .fd = writer, .events = POLLOUT, .revents = 0,
      };

      auto timeoutMS = ToUnixTimeoutMS(timeout);

      auto pollResult = RL_TEMP_FAILURE_RETRY(::poll(&pollFd, 1, timeoutMS));

      if (pollResult == 0) {
        /*
         *  Poll timeout expired
         */
        return IOResult::Timeout;
      }

      if (pollResult == 1) {
        /*
         *  Socket write is available
         */
        continue;
      }
    }

    break;
  }

  return sent == static_cast<int64_t>(expectedSendSize) ? IOResult::Success
                                                        : IOResult::Failure;
}

/*
 *  Temporary workaround till we can optimize sending multiple messages in one
 *  call. We already use scatter gather arrays, so should stick it in there.
 */
IOResult SocketChannel::writeMessageSingle(const Message& message,
                                           ClockDurationNano timeout) {
  /*
   *  Check if the message buffer is small enough to be sent inline
   */
  const auto isDataInline = message.size() < kMaxInlineBufferSize;

  const auto& attachments = message.attachments();
  auto oolDescriptors = attachments.size();

  std::unique_ptr<SharedMemory> oolMemoryArena;

  /*
   *  If the message cannot be sent inline, we allocate a shared memory arena
   *  large enough to hold the message and send the descriptor of that arena
   *  instead of the message.
   */
  if (!isDataInline) {
    oolMemoryArena = make_unique<SharedMemory>(message.size());

    if (!oolMemoryArena->isReady() ||
        oolMemoryArena->size() != message.size()) {
      /*
       *  We could not allocate an OOL memory arena to transfer the contents
       *  of this large message. We may be able to service this later though.
       */
      return IOResult::Timeout;
    }

    /*
     *  Copy the contents of the large message into the arena we are going to
     *  send the handle OOL for
     */
    memcpy(oolMemoryArena->address(), message.data(), message.size());

    /*
     *  The OOL memory arena takes up another descriptor
     */
    oolDescriptors++;
  }

  if (oolDescriptors > kMaxControlBufferItemCount) {
    /*
     *  This is too many descriptors for this implementation
     */
    return IOResult::Failure;
  }

  const size_t vecLength = isDataInline ? 2 : 1;

  SocketPayloadHeader header(isDataInline, oolDescriptors);

  struct iovec vec[vecLength];

  /*
   *  The first item in the scatter gather array is always the payload header
   */
  vec[0].iov_base = &header;
  vec[0].iov_len = sizeof(header);

  /*
   *  Inline message buffers (if present) are the second item in the array
   */
  if (vecLength == 2) {
    vec[1].iov_base = reinterpret_cast<void*>(message.data());
    vec[1].iov_len = message.size();
  }

  /*
   *  Create the message header containing the inline data descriptions and
   *  attachments
   */
  struct msghdr messageHeader = {
    .msg_name = nullptr,
    .msg_namelen = 0,
    .msg_iov = vec,
#if RL_OS_MAC
    .msg_iovlen = static_cast<int>(vecLength),
#else
    .msg_iovlen = vecLength,
#endif
    .msg_control = nullptr,
    .msg_controllen = 0,
    .msg_flags = 0,
  };

  Allocation controlBuffer;

  /*
   *  If there are any OOL descriptors (explicitly via attachments or OOL
   *  memory arenas), the control message needs to be initialized.
   */
  if (oolDescriptors > 0) {
    if (!controlBuffer.resize(
            CMSG_SPACE((oolDescriptors * sizeof(SocketPair::Handle))))) {
      /*
       *  We could not allocate enough memory on the client for the control
       *  buffer. Memory pressure may subside later. So Timeout.
       */
      return IOResult::Timeout;
    }

    messageHeader.msg_control = controlBuffer.data();
    messageHeader.msg_controllen = static_cast<socklen_t>(controlBuffer.size());

    struct cmsghdr* cmsg = CMSG_FIRSTHDR(&messageHeader);

    if (cmsg == nullptr) {
      return IOResult::Failure;
    }

    auto descCount = 0;
    SocketPair::Handle handles[oolDescriptors];

    for (const auto& attachment : message.attachments()) {
      handles[descCount++] =
          static_cast<SocketPair::Handle>(attachment->attachmentHandle());
    }

    if (!isDataInline) {
      /*
       *  The last descriptor is the memory arena handle (if OOL). The arena
       *  cannot be nullptr since we return early with a timeout in that case.
       */
      handles[descCount++] = oolMemoryArena->handle();
    }

    cmsg->cmsg_level = SOL_SOCKET;
    cmsg->cmsg_type = SCM_RIGHTS;
    cmsg->cmsg_len = static_cast<socklen_t>(
        CMSG_LEN(oolDescriptors * sizeof(SocketPair::Handle)));
    memcpy(CMSG_DATA(cmsg), handles, sizeof(handles));
  }

  const auto expectedSendSize =
      sizeof(SocketPayloadHeader) + (isDataInline ? message.size() : 0);

  auto result = SocketSendMessage(_pair->writeHandle(), &messageHeader,
                                  expectedSendSize, timeout);

  return result;
}

using RecvResult = std::pair<IOResult, ssize_t>;
/**
 *  Just like the POSIX `recvmsg` but accepts a timeout
 *
 *  @param handle  the handle to receive the message on
 *  @param header  the initialized msghdr struct
 *  @param flags   recvmsg flags
 *  @param timeout the timeout
 *
 *  @return result of the `recvmsg` and the size of the received bytes
 */
static RecvResult SocketChannelRecvMsg(int handle,
                                       struct msghdr* header,
                                       int flags,
                                       ClockDurationNano timeout) {
  ssize_t received = 0;
  while (true) {
    received = RL_TEMP_FAILURE_RETRY(::recvmsg(handle, header, flags));

    if (received == -1 && errno == EAGAIN) {
      /*
       *  The non blocking socket has nothing to receive. Depending on the
       *  timeout, try again
       */
      if (timeout.count() == 0) {
        /*
         *  Dont bother with a syscall for a zero timeout receive
         */
        return RecvResult(IOResult::Timeout, 0);
      }

      /*
       *  Poll on the socket for the given timeout
       */
      struct pollfd pollFd = {
          .fd = handle, .events = POLLIN, .revents = 0,
      };

      auto timeoutMS = ToUnixTimeoutMS(timeout);

      auto pollResult = RL_TEMP_FAILURE_RETRY(::poll(&pollFd, 1, timeoutMS));

      if (pollResult == 1) {
        /*
         *  Finally, the descriptor is available for reading. Try the recvmsg
         *  again
         */
        continue;
      }

      if (pollResult == 0) {
        /*
         *  We tried waiting on the descriptor but did not succeed to find
         *  anything to read
         */
        return RecvResult(IOResult::Timeout, 0);
      }

      /*
       *  Paranoid assertion to check that we did not mess up the poll
       */
      return RecvResult(IOResult::Failure, 0);
    }

    break;
  }

  return RecvResult(IOResult::Success, received);
}

AttachmentRef SocketChannel::attachment() {
  return _pair;
}

IOReadResult SocketChannel::readMessage(ClockDurationNano timeout) {
  std::lock_guard<std::mutex> lock(_readBufferMutex);

  /*
   *  ==========================================================================
   *  Initialize the message header used to make the `recvmsg` call on the
   *  socket
   *  ==========================================================================
   */
  struct iovec vec[2] = {};

  SocketPayloadHeader header;

  /*
   *  The first element in the scatter gather array is always the header
   */
  vec[0].iov_base = &header;
  vec[0].iov_len = sizeof(header);

  /*
   *  The second element in the scatter gather array contains space for the
   *  inline message buffer (if present)
   */
  vec[1].iov_base = _inlineMessageBuffer.data();
  vec[1].iov_len = _inlineMessageBuffer.size();

  _controlBuffer.makeZero();

  struct msghdr messageHeader = {
      .msg_name = nullptr,
      .msg_namelen = 0,
      .msg_iov = vec,
      .msg_iovlen = sizeof(vec) / sizeof(struct iovec),
      .msg_control = _controlBuffer.data(),
      .msg_controllen = static_cast<socklen_t>(_controlBuffer.size()),
      .msg_flags = 0,
  };

  /*
   *  ==========================================================================
   *  Make the `recvmsg` call on the socket
   *  ==========================================================================
   */

  auto result =
      SocketChannelRecvMsg(_pair->readHandle(), &messageHeader, 0, timeout);

  if (result.first != IOResult::Success) {
    return IOReadResult(result.first, Message{});
  }

  if ((messageHeader.msg_flags & (MSG_CTRUNC | MSG_TRUNC)) != 0) {
    /*
     *  The message or the control buffers we provided were not large enough
     *  for some reason. The only case in which SOCK_SEQPACKET receives may be
     *  truncated is because of this. We shouldn't ever hit this case because
     *  we validate data on the senders size. But this is a paranoid check.
     */
    return IOReadResult(IOResult::Failure, Message{});
  }

  ssize_t received = result.second;

  if (received < static_cast<ssize_t>(sizeof(SocketPayloadHeader))) {
    /*
     *  We always expect a header in any message.
     */
    return IOReadResult(IOResult::Failure, Message{});
  }

  /*
   *  ==========================================================================
   *  Read the contents of the message (with either inline or OOL memory buffer)
   *  and its attachments.
   *  ==========================================================================
   */
  auto totalDescriptors = header.oolDescriptors();
  auto inlineMessageSize = received - sizeof(SocketPayloadHeader);

  if (totalDescriptors > kMaxControlBufferItemCount) {
    /*
     *  We have reached the limits of our implementation. Bail.
     */
    return IOReadResult(IOResult::Failure, Message{});
  }

  if (!header.isDataInline() && inlineMessageSize != 0) {
    /*
     *  The header said that its data was not inline but we still managed to
     *  receive extra bytes over the socket. There seems to some
     *  inconsistency. Bail.
     */
    return IOReadResult(IOResult::Failure, Message{});
  }

  std::unique_ptr<SharedMemory> oolMemoryArena;
  std::vector<AttachmentRef> attachments;

  if (totalDescriptors > 0) {
    struct cmsghdr* cmsg = CMSG_FIRSTHDR(&messageHeader);

    if (cmsg == nullptr) {
      /*
       *  The message header and the actually received control buffer seem to
       *  disagree about how many attachments there are. Bail.
       */
      return IOReadResult(IOResult::Failure, Message{});
    }

    auto handles = reinterpret_cast<SocketPair::Handle*>(CMSG_DATA(cmsg));

    /*
     *  If there is inline data, all descriptors are attachments. If not, the
     *  last descriptor is the shared memory arena.
     */
    const auto oolMemoryArenaDescriptorIndex =
        header.isDataInline() ? -1 : totalDescriptors - 1;

    for (int i = 0; i < totalDescriptors; i++) {
      auto handle = handles[i];

      if (oolMemoryArenaDescriptorIndex == i) {
        /*
         *  We create a shared memory instance from the handle but make it not
         *  own this handle or its mapping. We then manually close the handle
         *  and create a message from the same with a vm allocated backing. This
         *  message unmaps the allocation when it is done.
         *
         *  This way, there are no copies and we can get rid of descriptor
         *  entirely.
         */
        oolMemoryArena = make_unique<SharedMemory>(
            handle, false /* does not own its handle or mapping */);
        RL_CHECK(::close(handle));
      } else {
        /*
         *  This is a regular channel attachment
         */
        attachments.emplace_back(std::make_shared<RawAttachment>(handle));
      }
    }
  }

  /*
   *  Create the message we will be returning to the caller
   */
  uint8_t* buffer = nullptr;
  size_t bufferLength = 0;
  bool vmDeallocate = false;

  /*
   *  Initialize the message data
   */
  if (header.isDataInline()) {
    buffer = _inlineMessageBuffer.data();
    bufferLength = inlineMessageSize;
  } else {
    if (oolMemoryArena == nullptr || !oolMemoryArena->isReady()) {
      /*
       *  The header said there was OOL data but we were not able to
       *  initialize the OOL arena
       */
      return IOReadResult(IOResult::Failure, Message{});
    }

    buffer = oolMemoryArena->address();
    bufferLength = oolMemoryArena->size();
    vmDeallocate = true;
  }

  Message message(buffer, bufferLength, vmDeallocate);

  for (auto& attachment : attachments) {
    if (!message.encode(std::move(attachment))) {
      RL_ASSERT_MSG(false, "Internal error: Will leak handles in message.");
      return IOReadResult(IOResult::Failure, Message{});
    }
  }

  return IOReadResult(IOResult::Success, std::move(message));
}

}  // namespace core
}  // namespace rl

static_assert(EAGAIN == EWOULDBLOCK,
              "EAGAIN should be the same as EWOULDBLOCK");

#endif  // RL_CHANNELS == RL_CHANNELS_SOCKET
