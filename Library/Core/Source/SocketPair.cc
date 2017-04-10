/*
 *  This source file is part of the Radar project.
 *  Licensed under the MIT License. See LICENSE file for details.
 */

#include "Config.h"

#if RL_CHANNELS == RL_CHANNELS_SOCKET

#include <fcntl.h>
#include <sys/socket.h>
#include <unistd.h>
#include "SocketPair.h"

namespace rl {
namespace core {

const SocketPair::Handle SocketPair::kInvalidHandle = -1;
const size_t SocketPair::kDefaultSocketBufferSize = 4096;

bool SocketPair::ConfigureSocketHandle(SocketPair::Handle handle,
                                       size_t bufferSize,
                                       bool blocking) {
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
   *  Make the socket non-blocking (if specified).
   */
  if (!blocking) {
    if (RL_TEMP_FAILURE_RETRY(::fcntl(handle, F_SETFL, O_NONBLOCK)) == -1) {
      RL_LOG_ERRNO();
      return false;
    }
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

  _isValid = ConfigureSocketHandle(_readHandle, bufferSize, false) &&
             ConfigureSocketHandle(_writeHandle, bufferSize, false);
}

SocketPair::SocketPair(RawAttachment attachment, size_t bufferSize)
    : _readHandle(kInvalidHandle),
      _writeHandle(kInvalidHandle),
      _isValid(false) {
  _writeHandle = attachment.takeHandle();
  _readHandle = RL_TEMP_FAILURE_RETRY(::dup(_writeHandle));

  _isValid = ConfigureSocketHandle(_readHandle, bufferSize, false) &&
             ConfigureSocketHandle(_writeHandle, bufferSize, false);
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
    RL_UNUSED(RL_TEMP_FAILURE_RETRY(::close(_readHandle)));
    _readHandle = kInvalidHandle;
  }

  if (_writeHandle != kInvalidHandle) {
    RL_UNUSED(RL_TEMP_FAILURE_RETRY(::close(_writeHandle)));
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
