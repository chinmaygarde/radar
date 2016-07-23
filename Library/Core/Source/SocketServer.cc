// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <Core/Config.h>

#if RL_CHANNELS == RL_CHANNELS_SOCKET

#include "SocketServer.h"

#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>

namespace rl {
namespace core {

SocketServer::SocketServer(const std::string& name,
                           bool clearPrevious,
                           AcceptCallback callback)
    : _handle(SocketPair::kInvalidHandle),
      _isValid(false),
      _acceptCallback(callback) {
  /*
   *  Check that the name looks valid.
   */
  auto nameLength = name.length();

  if (nameLength == 0 || nameLength > 64) {
    return;
  }

  /*
   *  If a previous entry exists, clear it if requested.
   */
  if (clearPrevious) {
    ::unlink(name.data());
  }

  /*
   *  Create the server socket handle;
   */
  _handle = ::socket(AF_UNIX, SOCK_SEQPACKET, 0);

  if (_handle == SocketPair::kInvalidHandle) {
    RL_LOG_ERRNO();
    return;
  }

  /*
   *  Create a new socket binding at the specified path.
   */

  struct sockaddr_un local = {};
  local.sun_family = AF_UNIX;
  strncpy(local.sun_path, name.data(), nameLength);

  auto len = static_cast<socklen_t>(sizeof(local.sun_family) + nameLength);
  auto localAddress = reinterpret_cast<struct sockaddr*>(&local);

  if (::bind(_handle, localAddress, len) != 0) {
    RL_LOG_ERRNO();
    return;
  }

  _isValid = SocketPair::ConfigureSocketHandle(_handle, 1024);

  setupAcceptSource();
}

SocketServer::~SocketServer() {
  if (_handle != SocketPair::kInvalidHandle) {
    RL_TEMP_FAILURE_RETRY(::close(_handle));
    _handle = SocketPair::kInvalidHandle;
  }
}

bool SocketServer::isValid() const {
  return _isValid;
}

bool SocketServer::listen(size_t backlog) const {
  return ::listen(_handle, backlog) == 0;
}

RawAttachment SocketServer::accept() const {
  struct sockaddr_un client = {};

  auto clientAddress = reinterpret_cast<struct sockaddr*>(&client);
  auto length = static_cast<socklen_t>(sizeof(client));

  SocketPair::Handle accepted =
      RL_TEMP_FAILURE_RETRY(::accept(_handle, clientAddress, &length));

  if (accepted == SocketPair::kInvalidHandle) {
    RL_LOG_ERRNO();
    return {};
  }

  return RawAttachment{accepted};
}

std::shared_ptr<EventLoopSource> SocketServer::source() {
  return _isValid ? _source : nullptr;
}

void SocketServer::setupAcceptSource() {
  if (!_isValid) {
    return;
  }

  auto handlesProvider = [&]() {
    return EventLoopSource::Handles{
        _handle,                     // read handle
        SocketPair::kInvalidHandle,  // write handle (disallowed)
    };
  };

  auto readHandler = [&](EventLoopSource::Handle readHandle) {
    RL_ASSERT(readHandle == _handle);

    auto attachment = SocketPair{accept()};

    if (_acceptCallback) {
      _acceptCallback(std::move(attachment));
    }

    return IOResult::Success;
  };

  _source =
      std::make_shared<EventLoopSource>(handlesProvider,  // handles provider
                                        nullptr,          // handles collector
                                        readHandler,      // read handler
                                        nullptr,          // write handler
                                        nullptr           // update handler
                                        );
}

}  // namespace core
}  // namespace rl

#endif  // RL_CHANNELS == RL_CHANNELS_SOCKET
