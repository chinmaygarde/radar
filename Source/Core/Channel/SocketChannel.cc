// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <Core/Channel/SocketChannel.h>
#include <Core/Utilities.h>
#include <Core/Message.h>

#if __APPLE__
// For Single Unix Standard v3 (SUSv3) conformance
#define _DARWIN_C_SOURCE
#endif

#include <mutex>
#include <fcntl.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/un.h>
#include <errno.h>
#include <poll.h>

namespace rl {

static const size_t MaxBufferSize = 4096;
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
  const int size = MaxBufferSize;

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
  _buffer = static_cast<uint8_t*>(malloc(MaxBufferSize));
  _controlBuffer = static_cast<uint8_t*>(malloc(MaxControlBufferSize));
}

SocketChannel::~SocketChannel() {
  free(_buffer);
  free(_controlBuffer);

  _buffer = nullptr;
  _controlBuffer = nullptr;
}

std::shared_ptr<LooperSource> SocketChannel::createSource() const {
  using LS = LooperSource;

  LS::RWHandlesProvider provider = [&]() {
    return LS::Handles(readHandle(), writeHandle()); /* bi-di connection */
  };

  LS::IOHandler readHandler =
      [this](LS::Handle handle) { _channel.readPendingMessageNow(); };

  /**
   *  We are specifying a null write handler since we will
   *  never directly signal this source. Instead, we will write
   *  to the handle directly.
   *
   *  The channel owns the socket handle, so there is no deallocation
   *  callback either.
   */
  return std::make_shared<LS>(provider, nullptr, readHandler, nullptr, nullptr);
}

bool SocketChannel::doTerminate() {
  SocketChannel_CloseHandle(readHandle());
  SocketChannel_CloseHandle(writeHandle());
  return false;
}

SocketChannel::Result SocketChannel::WriteMessages(const Messages& messages) {
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
  if (message.size() > MaxBufferSize) {
    return Result::TemporaryFailure;
  }

  struct iovec vec[1] = {{0}};

  vec[0].iov_base = (void*)message.data();
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

  long sent = 0;

  const Handle writer = writeHandle();

  while (true) {
    sent = RL_TEMP_FAILURE_RETRY(::sendmsg(writer, &messageHeader, 0));

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

  if (sent != message.size()) {
    return errno == EPIPE ? Result::PermanentFailure : Result::TemporaryFailure;
  }

  return Result::Success;
}

SocketChannel::ReadResult SocketChannel::ReadMessages() {
  std::lock_guard<std::mutex> lock(_lock);

  struct iovec vec[1] = {{0}};

  vec[0].iov_base = _buffer;
  vec[0].iov_len = MaxBufferSize;

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

    long received =
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
      /*
       *  if no messages are available to be received and the peer
       *  has performed an orderly shutdown, recvmsg() returns 0
       */
      return ReadResult(Result::PermanentFailure, std::move(messages));
    }
  }

  return ReadResult(Result::Success, std::move(messages));
}

SocketChannel::Handle SocketChannel::readHandle() const {
  return _handles.first;
}

SocketChannel::Handle SocketChannel::writeHandle() const {
  return _handles.second;
}

}  // namespace rl
