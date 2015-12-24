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

static const size_t MaxInlineBufferSize = 4096;
static const size_t MaxControlBufferItemCount = 8;
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

  Handle handle = static_cast<Handle>(attachment.handle());
  Handle other = static_cast<Handle>(RL_TEMP_FAILURE_RETRY(::dup(handle)));

  setupWithHandles(handle, other);
}

SocketChannel::SocketChannel(Channel& channel) : _channel(channel) {
  int socketHandles[2] = {0};

  RL_CHECK(::socketpair(AF_UNIX, SOCK_SEQPACKET, 0, socketHandles));

  setupWithHandles(socketHandles[0], socketHandles[1]);
}

void SocketChannel::setupWithHandles(Handle a, Handle b) {
  RL_ASSERT(a != b);

  SocketChannel_ConfigureHandle(a);
  SocketChannel_ConfigureHandle(b);

  _handles = std::make_pair(a, b);

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
    auto result = writeMessageSingle(message);
    if (result != IOResult::Success) {
      return result;
    }
  }

  return IOResult::Success;
}

/*
 *  Temporary workaround till we can optimize sending multiple messages in one
 *  call. We already use scatter gather arrays, so should stick it in there.
 */
IOResult SocketChannel::writeMessageSingle(const Message& message) {
  /*
   *  If the messages has an attachment, those take priority
   */
  if (message.attachment().isValid()) {
    auto handle =
        static_cast<SocketChannel::Handle>(message.attachment().handle());
    return writeDescriptorOutOfLine(handle, OOLDescriptor::Handle);
  }

  /*
   *  Check if the message is small enough to be sent inline or if a separate
   *  shared memory arena needs to be allocated for the transfer
   */
  if (message.size() < MaxInlineBufferSize) {
    return writeDataMessageInline(message);
  } else {
    return writeDataMessageOutOfLine(message);
  }
}

IOResult SocketSendMessage(SocketChannel::Handle writer,
                           struct msghdr* messageHeader,
                           size_t expectedSendSize) {
  int64_t sent = 0;
  while (true) {
    sent = RL_TEMP_FAILURE_RETRY(::sendmsg(writer, messageHeader, 0));

    if (sent == -1 && (errno == EAGAIN || errno == EWOULDBLOCK)) {
      struct pollfd pollFd = {
          .fd = writer, .events = POLLOUT, .revents = 0,
      };

      int res = RL_TEMP_FAILURE_RETRY(::poll(&pollFd, 1, -1 /* timeout */));

      RL_ASSERT(res == 1);
    } else {
      break;
    }
  }

  if (sent != expectedSendSize) {
    return IOResult::Failure;
  }

  return IOResult::Success;
}

IOResult SocketChannel::writeDataMessageInline(const Message& message) {
  struct iovec vec[1] = {{0}};

  /*
   *  Directly set the data to send as part of the scatter/gather array
   */
  vec[0].iov_base = reinterpret_cast<void*>(message.data());
  vec[0].iov_len = message.size();

  struct msghdr messageHeader = {
      .msg_name = nullptr,
      .msg_namelen = 0,
      .msg_iov = vec,
      .msg_iovlen = sizeof(vec) / sizeof(struct iovec),
      .msg_control = nullptr,
      .msg_controllen = 0,
      .msg_flags = 0,
  };

  return SocketSendMessage(writeHandle(), &messageHeader, message.size());
}

IOResult SocketChannel::writeDataMessageOutOfLine(const Message& message) {
  /*
   *  Create a shared memory region and copy over the contents of the message
   *  over to that region
   */
  SharedMemory memory(message.size());
  RL_ASSERT(memory.isReady() && memory.size() == message.size());
  memcpy(memory.address(), message.data(), message.size());

  return writeDescriptorOutOfLine(memory.handle(), OOLDescriptor::Data);
}

IOResult SocketChannel::writeDescriptorOutOfLine(
    SocketChannel::Handle descriptor,
    OOLDescriptor desc) {
  /*
   *  Theory of Operation:
   *
   *  Even out of line descriptors use the inline message buffer.
   *  Descriptors themselves are sent in the control buffer within the message
   *  flushed down the unix domain socket. The type of the descriptor however is
   *  sent in the message itself. The receiver interprets the type of the
   *  descriptor according to the value it sees in the inline message
   */

  auto descriptorType = static_cast<OOLDescriptorType>(desc);
  struct iovec vec[1] = {{0}};
  vec[0].iov_base = &descriptorType;
  vec[0].iov_len = sizeof(descriptorType);

  /*
   *  Create the message header structure containing the descriptor to send over
   *  the channel
   */
  const socklen_t controlBufferSize = CMSG_SPACE(sizeof(SocketChannel::Handle));
  void* controlBuffer = calloc(1, controlBufferSize);
  struct msghdr messageHeader = {
      .msg_name = nullptr,
      .msg_namelen = 0,
      .msg_iov = vec,
      .msg_iovlen = sizeof(vec) / sizeof(struct iovec),
      .msg_control = controlBuffer,
      .msg_controllen = controlBufferSize,
      .msg_flags = 0,
  };

  struct cmsghdr* cmsg = CMSG_FIRSTHDR(&messageHeader);
  cmsg->cmsg_level = SOL_SOCKET;
  cmsg->cmsg_type = SCM_RIGHTS;
  cmsg->cmsg_len = CMSG_LEN(sizeof(SocketChannel::Handle));
  *((SocketChannel::Handle*)CMSG_DATA(cmsg)) = descriptor;
  messageHeader.msg_controllen = cmsg->cmsg_len;

  auto result =
      SocketSendMessage(writeHandle(), &messageHeader, sizeof(descriptorType));

  free(controlBuffer);

  return result;
}

IOReadResult SocketChannel::readMessage(ClockDurationNano timeout) {
  std::lock_guard<std::mutex> lock(_readBufferMutex);

  struct iovec vec[1] = {{0}};

  vec[0].iov_base = _buffer;
  vec[0].iov_len = MaxInlineBufferSize;

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

  /*
   *  ==========================================================================
   *  Step 1: Check if there is an in-line buffer
   *  ==========================================================================
   */
  if (received > 0 && messageHeader.msg_controllen == 0) {
    Message message(_buffer, received);

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
  }

  /*
   *  ==========================================================================
   *  Step 2: Check if there is an out-of-line buffer
   *  ==========================================================================
   */
  if (received >= 0) {
    /*
     *  If no messages are available to be received and the peer
     *  has performed an orderly shutdown, and, there are no control messages,
     *  recvmsg() returns 0.
     */
    if (messageHeader.msg_controllen == 0) {
      goto PermanentFailure;
    }

    /*
     *  Read the type of descriptor we got. See the discussion on how this is
     *  obtained in the routine that sends the message down the socket
     */
    if (messageHeader.msg_iovlen != 1) {
      goto PermanentFailure;
    }

    if (received != sizeof(OOLDescriptorType)) {
      goto PermanentFailure;
    }

    auto descBuffer = reinterpret_cast<OOLDescriptorType*>(_buffer);
    auto desc = static_cast<OOLDescriptor>(*descBuffer);

    /*
     *  Read the descriptor itself from the buffer
     */
    struct cmsghdr* cmsg = CMSG_FIRSTHDR(&messageHeader);

    if (cmsg == nullptr) {
      goto PermanentFailure;
    }

    SocketChannel::Handle handle = *((SocketChannel::Handle*)CMSG_DATA(cmsg));

    return readFromHandle(handle, desc);
  }

/*
 *  ============================================================================
 *  Step 3: All other errors are fatal
 *  ============================================================================
 */

PermanentFailure: /* :( */
  return IOReadResult(IOResult::Failure, Message{});
}

IOReadResult SocketChannel::readFromHandle(SocketChannel::Handle handle,
                                           OOLDescriptor desc) {
  switch (desc) {
    case OOLDescriptor::Data: {
      /*
       *  We create a shared memory instance from the handle but make it not
       *  own its handle or the address mapping. We then manually close the
       *  handle and create a message from the same with a "vm allocated"
       *  backing. This message unmaps the allocation when it is done.
       *
       *  This way, there are no more copies!
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
    } break;
    case OOLDescriptor::Handle: {
      auto attachment = static_cast<Message::Attachment::Handle>(handle);
      return IOReadResult(IOResult::Success,
                          Message{Message::Attachment{attachment}});
    } break;
    default:
      RL_LOG("Unexpected descriptor type: %d",
             static_cast<OOLDescriptorType>(desc));
      break;
  }

  return IOReadResult(IOResult::Failure, Message{});
}

Message::Attachment::Handle SocketChannel::handle() {
  return _handles.first;
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
