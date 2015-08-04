// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "Core/Channel/SocketChannel.h"
#include "Core/Utilities.h"
#include "Core/Message.h"

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
static const std::string DefaultConnectedPairName = "rl.connected";

Channel::ConnectedPair SocketChannel::CreateConnectedPair() {
  int socketHandles[2] = {0};

  RL_CHECK(::socketpair(AF_UNIX, SOCK_SEQPACKET, 0, socketHandles));

  return ConnectedPair(Shared(DefaultConnectedPairName, socketHandles[0]),
                       Shared(DefaultConnectedPairName, socketHandles[1]));
}

SocketChannel::SocketChannel(const std::string& name) : SocketChannel(name, 0) {
}

SocketChannel::SocketChannel(const std::string& name, Handle handle)
    : Channel(name) {
  /*
   *  Create a socket handle if one is not provided
   */

  if (handle == 0) {
    handle = ::socket(AF_UNIX, SOCK_SEQPACKET, 0);
    RL_ASSERT(handle != -1);
  }

  RL_ASSERT(_handle > 0);
  _handle = handle;

  /*
   *  Limit the socket send and receive buffer sizes since we dont need large
   *  buffers for channels
   */
  const int size = MaxBufferSize;

  RL_CHECK(::setsockopt(_handle, SOL_SOCKET, SO_SNDBUF, &size, sizeof(size)));

  RL_CHECK(::setsockopt(_handle, SOL_SOCKET, SO_RCVBUF, &size, sizeof(size)));

  /*
   *  Make the sockets non blocking since we plan to receive all messages
   *  instead of hitting the waitset for each message in the queue. This is
   *  important since reads will block if we dont.
   */
  RL_CHECK(::fcntl(_handle, F_SETFL, O_NONBLOCK));

  RL_CHECK(::fcntl(_handle, F_SETFL, O_NONBLOCK));

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

std::shared_ptr<LooperSource> SocketChannel::source() {
  if (_source.get() != nullptr) {
    return _source;
  }

  RL_ASSERT(isConnected() == true);

  using LS = LooperSource;

  LS::IOHandlesAllocator allocator = [&]() {
    return LS::Handles(_handle, _handle); /* bi-di connection */
  };

  LS::IOHandler readHandler =
      [this](LS::Handle handle) { readPendingMessageNow(); };

  /**
   *  We are specifying a null write handler since we will
   *  never directly signal this source. Instead, we will write
   *  to the handle directly.
   *
   *  The channel owns the socket handle, so there is no deallocation
   *  callback either.
   */
  _source = std::make_shared<LS>(allocator, nullptr, readHandler, nullptr);

  return _source;
}

bool SocketChannel::doConnect(const std::string& endpoint) {
  /*
   *  Connect
   */
  RL_ASSERT(endpoint.length() <= 96);

  struct sockaddr_un addr = {0};
  addr.sun_family = AF_UNIX;
  strcpy(addr.sun_path, endpoint.c_str());

#if __APPLE__
  addr.sun_len = SUN_LEN(&addr);
#endif

  int result = ::connect(_handle, (const struct sockaddr*)&addr,
                         (socklen_t)SUN_LEN(&addr));

  return (result == 0);
}

bool SocketChannel::doTerminate() {
  if (_handle != -1) {
    RL_CHECK(::close(_handle));
    _handle = -1;

    return true;
  }

  return false;
}

SocketChannel::Result SocketChannel::WriteMessage(Message& message) {
  std::lock_guard<std::mutex> lock(_lock);

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

  while (true) {
    sent = RL_TEMP_FAILURE_RETRY(::sendmsg(_handle, &messageHeader, 0));

    if (sent == -1 && (errno == EAGAIN || errno == EWOULDBLOCK)) {
      struct pollfd pollFd = {
          .fd = _handle, .events = POLLOUT, .revents = 0,
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

  std::vector<std::unique_ptr<Message>> messages;

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
        RL_TEMP_FAILURE_RETRY(::recvmsg(_handle, &messageHeader, 0));

    if (received > 0) {
      auto message = rl::Utils::make_unique<Message>(_buffer, received);

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

template <typename... T>
std::shared_ptr<SocketChannel> SocketChannel::Shared(T&&... args) {
  struct SharedSocketChannel : public SocketChannel {
    SharedSocketChannel(T&&... args)
        : SocketChannel(std::forward<T>(args)...) {}
  };
  return std::make_shared<SharedSocketChannel>(std::forward<T>(args)...);
}

}  // namespace rl
