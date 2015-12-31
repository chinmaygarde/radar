// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <Core/Config.h>

#if RL_CHANNELS == RL_CHANNELS_SOCKET

#include <Core/Message.h>
#include <Core/SharedMemory.h>
#include <Core/SocketChannel.h>
#include <Core/Utilities.h>

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

/**
 *  This header is always the first item present in the scatter gather array
 *  used in all socket payloads.
 */
class SocketPayloadHeader {
 public:
  SocketPayloadHeader() : SocketPayloadHeader(false, 0) {}

  SocketPayloadHeader(bool isDataInline, uint8_t oolDescriptors)
      : _isDataInline(isDataInline), _oolDescriptors(oolDescriptors) {}

  uint8_t oolDescriptors() const { return _oolDescriptors; }

  bool isDataInline() const { return _isDataInline; };

 private:
  uint8_t _oolDescriptors;
  bool _isDataInline;

  RL_DISALLOW_COPY_AND_ASSIGN(SocketPayloadHeader);
};

static_assert(rl_trivially_copyable(SocketPayloadHeader),
              "The socket payload must be trivially copyable");

static const size_t MaxInlineBufferSize = 4096;
static const size_t MaxControlBufferItemCount = 24;
static const size_t ControlBufferItemSize = sizeof(int);
static const size_t MaxControlBufferSize =
    CMSG_SPACE(ControlBufferItemSize * MaxControlBufferItemCount);

static void SocketChannel_ConfigureHandle(SocketChannel::Handle handle) {
  RL_ASSERT(handle > 0);

  /*
   *  Limit the socket send and receive buffer sizes since we dont need large
   *  buffers for channels
   */
  const int size = MaxInlineBufferSize;

  RL_CHECK(::setsockopt(handle, SOL_SOCKET, SO_SNDBUF, &size, sizeof(size)));
  RL_CHECK(::setsockopt(handle, SOL_SOCKET, SO_RCVBUF, &size, sizeof(size)));

  /*
   *  Make sockets non blocking. We will explicitly poll if reads or writes with
   *  timeouts need to be serviced.
   */
  RL_CHECK(::fcntl(handle, F_SETFL, O_NONBLOCK));
}

static bool SocketChannel_CloseHandle(SocketChannel::Handle handle) {
  if (handle != -1) {
    RL_CHECK(::close(handle));
    return true;
  }
  return false;
}

SocketChannel::SocketChannel(Channel& channel,
                             const Message::Attachment& attachment)
    : _channel(channel) {
  RL_ASSERT_MSG(attachment.isValid(),
                "Can only create channels from valid message attachments");

  Handle writeHandle = static_cast<Handle>(attachment.handle());
  Handle readHandle =
      static_cast<Handle>(RL_TEMP_FAILURE_RETRY(::dup(writeHandle)));

  setupWithHandles(readHandle, writeHandle);
}

SocketChannel::SocketChannel(Channel& channel) : _channel(channel) {
  int socketHandles[2] = {0};

  RL_CHECK(::socketpair(AF_UNIX, SOCK_SEQPACKET, 0, socketHandles));

  setupWithHandles(socketHandles[0], socketHandles[1]);
}

void SocketChannel::setupWithHandles(Handle readHandle, Handle writeHandle) {
  RL_ASSERT(readHandle != writeHandle);

  SocketChannel_ConfigureHandle(readHandle);
  SocketChannel_ConfigureHandle(writeHandle);

  _handles = std::make_pair(readHandle, writeHandle);

  /*
   *  Setup the channel buffer
   */
  _buffer = static_cast<uint8_t*>(malloc(MaxInlineBufferSize));
  _controlBuffer = static_cast<uint8_t*>(malloc(MaxControlBufferSize));
}

SocketChannel::~SocketChannel() {
  free(_buffer);
  free(_controlBuffer);

  _buffer = nullptr;
  _controlBuffer = nullptr;
}

std::shared_ptr<EventLoopSource> SocketChannel::createSource() const {
  using ELS = EventLoopSource;

  ELS::RWHandlesProvider provider = [&]() {
    return ELS::Handles(readHandle(), writeHandle()); /* bi-di connection */
  };

  ELS::IOHandler readHandler = [this](ELS::Handle handle) {
    return _channel.readPendingMessageNow();
  };

  /**
   *  We are specifying a null write handler since we will
   *  never directly signal this source. Instead, we will write
   *  to the handle directly.
   *
   *  The channel owns the socket handle, so there is no deallocation
   *  callback either.
   */
  return std::make_shared<ELS>(provider, nullptr, readHandler, nullptr,
                               nullptr);
}

bool SocketChannel::doTerminate() {
  auto readClosed = SocketChannel_CloseHandle(readHandle());
  auto writeClosed = SocketChannel_CloseHandle(writeHandle());
  return readClosed && writeClosed;
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

static IOResult SocketSendMessage(SocketChannel::Handle writer,
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

  return sent == expectedSendSize ? IOResult::Success : IOResult::Failure;
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
  const auto isDataInline = message.size() < MaxInlineBufferSize;

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

  if (oolDescriptors >= MaxControlBufferItemCount) {
    /*
     *  This is too many descriptors for this implementation
     */
    return IOResult::Failure;
  }

  const auto vecLength = isDataInline ? 2 : 1;

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
   *  Create the message header structure containing the descriptors (if any
   *  to send over the channel
   */
  void* controlBuffer = nullptr;
  size_t controlBufferSize = 0;

  if (oolDescriptors > 0) {
    controlBufferSize =
        CMSG_SPACE((oolDescriptors * sizeof(SocketChannel::Handle)));
    controlBuffer = calloc(1, controlBufferSize);

    if (controlBuffer == nullptr) {
      /*
       *  We ran out of client memory to service this write. We may be able to
       *  service this request later however.
       */
      return IOResult::Timeout;
    }
  }

  struct msghdr messageHeader = {
      .msg_name = nullptr,
      .msg_namelen = 0,
      .msg_iov = vec,
      .msg_iovlen = vecLength,
      .msg_control = controlBuffer,
      .msg_controllen = static_cast<socklen_t>(controlBufferSize),
      .msg_flags = 0,
  };

  /*
   *  If there are any OOL descriptors (explicitly via attachments or OOL
   *  memory arenas), the control message needs to be initialized.
   */
  if (oolDescriptors > 0) {
    struct cmsghdr* cmsg = CMSG_FIRSTHDR(&messageHeader);
    cmsg->cmsg_level = SOL_SOCKET;
    cmsg->cmsg_type = SCM_RIGHTS;
    cmsg->cmsg_len = CMSG_LEN(sizeof(SocketChannel::Handle));

    auto descriptors =
        reinterpret_cast<SocketChannel::Handle*>(CMSG_DATA(cmsg));

    /*
     *  All the explicitly OOL descriptors come first
     */
    for (auto i = 0; i < oolDescriptors - 1; i++) {
      descriptors[i] =
          static_cast<SocketChannel::Handle>(attachments[i].handle());
    }

    if (!isDataInline) {
      /*
       *  The last descriptor is the memory arena handle (if OOL). The arena
       *  cannot be nullptr since we return early with a timeout in that case.
       */
      descriptors[oolDescriptors - 1] = oolMemoryArena->handle();
    }

    messageHeader.msg_controllen = cmsg->cmsg_len;
  }

  const auto expectedSendSize =
      sizeof(SocketPayloadHeader) + (isDataInline ? message.size() : 0);

  auto result = SocketSendMessage(writeHandle(), &messageHeader,
                                  expectedSendSize, timeout);

  free(controlBuffer);

  return result;
}

static inline cmsghdr* SocketChannelNextCmsgHdr(msghdr* hdr, cmsghdr* cmsg) {
  /*
   *  RFC 2292 (https://tools.ietf.org/html/rfc2292#section-4.3.2) says
   *  CMSG_NXTHDR should return CMSG_FIRSTHDR on null cmsg. This does not seem
   *  to be the case on Linux. So define our own version.
   */
  return cmsg == nullptr ? CMSG_FIRSTHDR(hdr) : CMSG_NXTHDR(hdr, cmsg);
}

IOReadResult SocketChannel::readMessage(ClockDurationNano timeout) {
  std::lock_guard<std::mutex> lock(_readBufferMutex);

  struct iovec vec[2] = {{0}};

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
  vec[1].iov_base = _buffer;
  vec[1].iov_len = MaxInlineBufferSize;

  bzero(_controlBuffer, MaxControlBufferItemCount);

  struct msghdr messageHeader = {
      .msg_name = nullptr,
      .msg_namelen = 0,
      .msg_iov = vec,
      .msg_iovlen = sizeof(vec) / sizeof(struct iovec),
      .msg_control = _controlBuffer,
      .msg_controllen = static_cast<socklen_t>(MaxControlBufferSize),
      .msg_flags = 0,
  };

  /*
   *  ==========================================================================
   *  Step 0: Make the call to `recvmsg`. Optionally, if a timeout is specified,
   *          poll on read availability on the socket for the given duration.
   *  ==========================================================================
   */
  ssize_t received = 0;
  while (true) {
    received =
        RL_TEMP_FAILURE_RETRY(::recvmsg(readHandle(), &messageHeader, 0));

    if (received == -1 && errno == EAGAIN) {
      /*
       *  The non blocking socket has nothing to receive. Depending on the
       *  timeout, try again
       */
      if (timeout.count() == 0) {
        /*
         *  Dont bother with a syscall for a zero timeout receive
         */
        return IOReadResult(IOResult::Timeout, Message{});
      }

      /*
       *  Poll on the socket for the given timeout
       */
      struct pollfd pollFd = {
          .fd = readHandle(), .events = POLLIN, .revents = 0,
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
        return IOReadResult(IOResult::Timeout, Message{});
      }

      /*
       *  Paranoid assertion to check that we did not mess up the poll
       */
      goto PermanentFailure;
    }

    break;
  }

  if (received < sizeof(SocketPayloadHeader)) {
    /*
     *  We always expect a header in any message.
     */
    goto PermanentFailure;
  }

  /*
   *  ==========================================================================
   *  Step 1: Check if there is an in-line buffer with no attachments. This is
   *          by far the most common case.
   *  ==========================================================================
   */
  if (messageHeader.msg_controllen == 0) {
    Message message(_buffer, received - sizeof(SocketPayloadHeader));

    if (header.oolDescriptors() != 0) {
      /*
       *  Something odd has happened. We did not get any control messages but
       *  the header says we should have some. There is some inconsistency here.
       *  Bail!
       */
      goto PermanentFailure;
    }

    /*
     *  Since we dont handle partial writes of control messages,
     *  assert that the message was not truncated.
     */
    if ((messageHeader.msg_flags & MSG_CTRUNC) != 0) {
      goto PermanentFailure;
    }

    /*
     *  Finally! We have the entire inline message and have verified that there
     *  were no attachments
     */
    return IOReadResult(IOResult::Success, std::move(message));
  } else {
    /*
     *  ========================================================================
     *  Step 2: The message has attachments (as well as possibly an OOL memory
     *          arena)
     *  ========================================================================
     */

    auto totalDescriptors = header.oolDescriptors();

    if (totalDescriptors < 1) {
      /*
       *  The presence of a control buffer indicates that the message contains
       *  attachments. However, the header disagrees. There is some
       *  inconsistency here. Bail.
       */
      goto PermanentFailure;
    }

    if (totalDescriptors > MaxControlBufferItemCount) {
      /*
       *  We have reached the limits of our implementation. Bail.
       */
      goto PermanentFailure;
    }

    /*
     *  If there is inline data, all descriptors are attachments. If not, the
     *  last descriptor is the shared memory arena.
     */
    auto attachmentDescriptors =
        header.isDataInline() ? totalDescriptors : totalDescriptors - 1;

    std::vector<Message::Attachment> attachments;

    struct cmsghdr* cmsg = nullptr;

    for (auto i = 0; i < attachmentDescriptors; i++) {
      /*
       *  CMSG_NXTHDR(mhdr, NULL) is equivalent to CMSG_FIRSTHDR(mhdr)
       */
      cmsg = SocketChannelNextCmsgHdr(&messageHeader, cmsg);

      if (cmsg == nullptr) {
        /*
         *  This will leak handles that were read previously.
         */
        goto PermanentFailure;
      }

      auto handle = *((SocketChannel::Handle*)CMSG_DATA(cmsg));
      attachments.push_back(handle);
    }

    /*
     *  Attempt to read the OOL memory arena if one is present
     */
    if (!header.isDataInline()) {
      cmsg = SocketChannelNextCmsgHdr(&messageHeader, cmsg);

      if (cmsg == nullptr) {
        /*
         *  The header indicated that there was an OOL memory arena descriptor
         *  We must be able to access its handle here.
         */
        goto PermanentFailure;
      }

      SocketChannel::Handle handle = *((SocketChannel::Handle*)CMSG_DATA(cmsg));
      /*
       *  We create a shared memory instance from the handle but make it not
       *  own its handle or the address mapping. We then manually close the
       *  handle and create a message from the same with a vm allocated
       *  backing. This message unmaps the allocation when it is done.
       *
       *  This way, there are no copies and we can get rid of descriptor
       *  entirely.
       */
      SharedMemory memory(handle, false /* assume ownership */);
      if (memory.isReady()) {
        /*
         *  The isReady check is equivalent to an EBADF guard
         */
        RL_CHECK(::close(handle));
        return IOReadResult(IOResult::Success,
                            Message{memory.address(), memory.size(), true});
      }
    }
  }

/*
 *  ============================================================================
 *  Step 3: All other errors are fatal
 *  ============================================================================
 */
PermanentFailure: /* :( */
  return IOReadResult(IOResult::Failure, Message{});
}

Message::Attachment::Handle SocketChannel::handle() {
  return writeHandle();
}

SocketChannel::Handle SocketChannel::readHandle() const {
  return _handles.first;
}

SocketChannel::Handle SocketChannel::writeHandle() const {
  return _handles.second;
}

}  // namespace core
}  // namespace rl

static_assert(EAGAIN == EWOULDBLOCK,
              "EAGAIN should be the same as EWOULDBLOCK");

#endif  // RL_CHANNELS == RL_CHANNELS_SOCKET
