// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "Config.h"

#if RL_CHANNELS == RL_CHANNELS_SOCKET

#include "SocketPair.h"

#include <sys/socket.h>
#include <fcntl.h>
#include <unistd.h>

namespace rl {
namespace core {

const SocketPair::Handle SocketPair::kInvalidHandle = -1;
const size_t SocketPair::kDefaultSocketBufferSize = 4096;

bool SocketPair::ConfigureSocketHandle(SocketPair::Handle handle,
                                       size_t bufferSize) {
  const int size = bufferSize;
  /*
   *  Update the send buffer size.
   */
  if (::setsockopt(handle, SOL_SOCKET, SO_SNDBUF, &size, sizeof(size)) != 0) {
    RL_LOG_ERRNO();
    return false;
  }

  /*
   *  Update the receive buffer size.
   */
  if (::setsockopt(handle, SOL_SOCKET, SO_RCVBUF, &size, sizeof(size)) != 0) {
    RL_LOG_ERRNO();
    return false;
  }

  /*
   *  Make the socket non-blocking.
   */
  if (RL_TEMP_FAILURE_RETRY(::fcntl(handle, F_SETFL, O_NONBLOCK)) == -1) {
    RL_LOG_ERRNO();
    return false;
  }

  return true;
}

SocketPair::SocketPair(size_t bufferSize)
    : _readHandle(kInvalidHandle),
      _writeHandle(kInvalidHandle),
      _isValid(false) {
  Handle handles[] = {kInvalidHandle, kInvalidHandle};

  if (::socketpair(AF_UNIX, SOCK_SEQPACKET, 0, handles) != 0) {
    RL_LOG_ERRNO();
    return;
  }

  _readHandle = handles[0];
  _writeHandle = handles[1];

  _isValid = ConfigureSocketHandle(_readHandle, bufferSize) &&
             ConfigureSocketHandle(_writeHandle, bufferSize);
}

SocketPair::SocketPair(RawAttachment attachment, size_t bufferSize)
    : _readHandle(kInvalidHandle),
      _writeHandle(kInvalidHandle),
      _isValid(false) {
  _writeHandle = attachment.takeHandle();
  _readHandle = RL_TEMP_FAILURE_RETRY(::dup(_writeHandle));

  _isValid = ConfigureSocketHandle(_readHandle, bufferSize) &&
             ConfigureSocketHandle(_writeHandle, bufferSize);
}

SocketPair::SocketPair(SocketPair&& o)
    : _readHandle(o._readHandle),
      _writeHandle(o._writeHandle),
      _isValid(o._isValid) {
  o._readHandle = kInvalidHandle;
  o._writeHandle = kInvalidHandle;
  o._isValid = false;
}

SocketPair::~SocketPair() {
  if (_readHandle != kInvalidHandle) {
    RL_TEMP_FAILURE_RETRY(::close(_readHandle));
    _readHandle = kInvalidHandle;
  }

  if (_writeHandle != kInvalidHandle) {
    RL_TEMP_FAILURE_RETRY(::close(_writeHandle));
    _writeHandle = kInvalidHandle;
  }
}

SocketPair::Handle SocketPair::readHandle() const {
  return _readHandle;
}

SocketPair::Handle SocketPair::writeHandle() const {
  return _writeHandle;
}

bool SocketPair::isValid() const {
  return _isValid;
}

Attachment::Handle SocketPair::handle() const {
  return _writeHandle;
}

}  // namespace core
}  // namespace rl

#endif  // RL_CHANNELS == RL_CHANNELS_SOCKET
