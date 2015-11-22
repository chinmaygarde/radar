// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <Core/Config.h>

#if RL_CHANNELS == RL_CHANNELS_SOCKET

#include <Core/SocketChannel.h>
#include <Core/Utilities.h>
#include <Core/Message.h>
#include <Core/SharedMemory.h>

#include <fcntl.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <errno.h>
#include <poll.h>
#include <stdlib.h>

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
   *  Make the sockets non blocking since we plan to receive all messages
   *  instead of hitting the waitset for each message in the queue. This is
   *  important since reads will block if we dont.
   */
  RL_CHECK(::fcntl(handle, F_SETFL, O_NONBLOCK));

  RL_CHECK(::fcntl(handle, F_SETFL, O_NONBLOCK));
}

static bool SocketChannel_CloseHandle(SocketChannel::Handle handle) {
  if (handle != -1) {
    RL_CHECK(::close(handle));
    return true;
  }
  return false;
}

SocketChannel::SocketChannel(Channel& channel) : _channel(channel) {
  int socketHandles[2] = {0};

  RL_CHECK(::socketpair(AF_UNIX, SOCK_SEQPACKET, 0, socketHandles));

  SocketChannel_ConfigureHandle(socketHandles[0]);
  SocketChannel_ConfigureHandle(socketHandles[1]);

  _handles = std::make_pair(socketHandles[0], socketHandles[1]);

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

  ELS::IOHandler readHandler =
      [this](ELS::Handle handle) { _channel.readPendingMessageNow(); };

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
  bool readClosed = SocketChannel_CloseHandle(readHandle());
  bool writeClosed = SocketChannel_CloseHandle(writeHandle());
  return readClosed && writeClosed;
}

SocketChannel::Result SocketChannel::WriteMessages(Messages&& messages,
                                                   ClockDurationNano timeout) {
  std::lock_guard<std::mutex> lock(_lock);

  for (const auto& message : messages) {
    Result res = writeMessageSingle(message);
    if (res != Result::Success) {
      return res;
    }
  }

  return Result::Success;
}

/*
 *  Temporary workaround till we can optimize sending multiple messages in one
 *  call. We already use scatter gather arrays, so should stick it in there.
 */
ChannelProvider::Result SocketChannel::writeMessageSingle(
    const Message& message) {
  if (message.size() < MaxInlineBufferSize) {
    return writeMessageInline(message);
  } else {
    return writeMessageOutOfLine(message);
  }
}

ChannelProvider::Result SocketSendMessage(SocketChannel::Handle writer,
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
    RL_LOG_ERRNO();
    return errno == EPIPE ? ChannelProvider::Result::PermanentFailure
                          : ChannelProvider::Result::TemporaryFailure;
  }

  return ChannelProvider::Result::Success;
}

ChannelProvider::Result SocketChannel::writeMessageInline(
    const Message& message) {
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

ChannelProvider::Result SocketChannel::writeMessageOutOfLine(
    const Message& message) {
  /*
   *  Create a shared memory region and copy over the contents of the message
   *  over to that region
   */
  SharedMemory memory(message.size());
  RL_ASSERT(memory.isReady() && memory.size() == message.size());
  memcpy(memory.address(), message.data(), message.size());

  /*
   *  Create the message header structure containing the descriptor to send over
   *  the channel
   */
  const socklen_t controlBufferSize = CMSG_SPACE(sizeof(SharedMemory::Handle));
  void* controlBuffer = calloc(1, controlBufferSize);
  struct msghdr messageHeader = {
      .msg_name = nullptr,
      .msg_namelen = 0,
      .msg_iov = nullptr,
      .msg_iovlen = 0,
      .msg_control = controlBuffer,
      .msg_controllen = controlBufferSize,
      .msg_flags = 0,
  };

  struct cmsghdr* cmsg = CMSG_FIRSTHDR(&messageHeader);
  cmsg->cmsg_level = SOL_SOCKET;
  cmsg->cmsg_type = SCM_RIGHTS;
  cmsg->cmsg_len = CMSG_LEN(sizeof(SharedMemory::Handle));
  *((SharedMemory::Handle*)CMSG_DATA(cmsg)) = memory.handle();
  messageHeader.msg_controllen = cmsg->cmsg_len;

  auto result = SocketSendMessage(writeHandle(), &messageHeader, 0);

  free(controlBuffer);

  return result;
}

SocketChannel::ReadResult SocketChannel::ReadMessages(
    ClockDurationNano timeout) {
  std::lock_guard<std::mutex> lock(_lock);

  struct iovec vec[1] = {{0}};

  vec[0].iov_base = _buffer;
  vec[0].iov_len = MaxInlineBufferSize;

  Messages messages;

  while (true) {
    struct msghdr messageHeader = {
        .msg_name = nullptr,
        .msg_namelen = 0,
        .msg_iov = vec,
        .msg_iovlen = sizeof(vec) / sizeof(struct iovec),
        .msg_control = _controlBuffer,
        .msg_controllen = static_cast<socklen_t>(MaxControlBufferSize),
        .msg_flags = 0,
    };

    int64_t received =
        RL_TEMP_FAILURE_RETRY(::recvmsg(readHandle(), &messageHeader, 0));

    if (received > 0) {
      Message message(_buffer, received);

      /*
       *  We do not support sending message attachments yet. Assert the same.
       */
      RL_ASSERT(messageHeader.msg_controllen == 0);

      /*
       *  Since we dont handle partial writes of the control messages,
       *  assert that the same was not truncated.
       */
      RL_ASSERT((messageHeader.msg_flags & MSG_CTRUNC) == 0);

      /*
       *  Finally! We have the message and possible attachments. Try for
       *  more.
       */
      messages.push_back(std::move(message));

      continue;
    }

    if (received == -1) {
      /*
       *  All pending messages have been read. poll for more
       *  in subsequent calls. We are finally done!
       */
      if (errno == EAGAIN || errno == EWOULDBLOCK) {
        /*
         *  Return as a successful read
         */
        break;
      }

      return ReadResult(Result::TemporaryFailure, std::move(messages));
    }

    if (received == 0) {
      if (messageHeader.msg_controllen == 0) {
        /*
         *  If no messages are available to be received and the peer
         *  has performed an orderly shutdown, recvmsg() returns 0
         */
        return ReadResult(Result::PermanentFailure, std::move(messages));
      } else {
        /*
         *  We only ever send either inline data or a descriptor. An OOL shared
         *  memory descriptor is present in this message. Read that!
         */
        struct cmsghdr* cmsg = CMSG_FIRSTHDR(&messageHeader);
        RL_ASSERT(cmsg != nullptr);
        SharedMemory::Handle handle = *((SharedMemory::Handle*)CMSG_DATA(cmsg));

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
          Message message(memory.address(), memory.size(), true);
          messages.push_back(std::move(message));
          continue;
        }
      }
    }
  }

  RL_ASSERT(messages.size() > 0);
  return ReadResult(Result::Success, std::move(messages));
}

SocketChannel::Handle SocketChannel::readHandle() const {
  return _handles.first;
}

SocketChannel::Handle SocketChannel::writeHandle() const {
  return _handles.second;
}

}  // namespace core
}  // namespace rl

#endif  // RL_CHANNELS == RL_CHANNELS_SOCKET
