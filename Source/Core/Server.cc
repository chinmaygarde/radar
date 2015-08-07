// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <Core/Server.h>
#include <Core/Utilities.h>

#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>

namespace rl {

Server::Server(std::string name)
    : _name(name), _socketHandle(-1), _listening(false) {
  /*
   *  Step 1: Create the socket
   */
  int socketHandle = socket(AF_UNIX, SOCK_SEQPACKET, 0);

  if (socketHandle == -1) {
    RL_LOG_ERRNO();
    RL_ASSERT(false);
    return;
  }

  RL_ASSERT(name.length() <= 96);

  /*
   *  Step 2: Bind
   */
  struct sockaddr_un addr = {0};
  addr.sun_family = AF_UNIX;
  strcpy(addr.sun_path, name.c_str());

#if __APPLE__
  addr.sun_len = SUN_LEN(&addr);
#endif

  int result = 0;

  /*
   *  In case of crashes or incorrect collection of resource, a previous
   *  FS entry might be present. Clear that as seen.
   */
  while (true) {
    /*
     *  Does not return return EINTR (at least what docs say)
     */
    int result = ::bind(socketHandle, (const struct sockaddr*)&addr,
                        (socklen_t)SUN_LEN(&addr));

    if (result == 0) {
      break;
    }

    if (result == -1 && errno == EADDRINUSE) {
      RL_CHECK(::unlink(name.c_str()));
    }
  }

  if (result == -1) {
    RL_LOG_ERRNO();
    RL_ASSERT(false);
    return;
  }

  /*
   *  Step 3: Listen
   */
  result = ::listen(socketHandle, 1);

  if (result == -1) {
    RL_LOG_ERRNO();
    RL_ASSERT(false);
    return;
  }

  _listening = true;
  _socketHandle = socketHandle;
}

Server::~Server() {
  if (_socketHandle != -1) {
    RL_CHECK(::unlink(_name.c_str()));
    RL_CHECK(::close(_socketHandle));
  }
}

void Server::onConnectionAvailableForAccept(Handle handle) {
  RL_ASSERT(handle == _socketHandle);

  Channel::Handle connectionHandle = ::accept(_socketHandle, nullptr, nullptr);

  if (connectionHandle == -1) {
    RL_LOG_ERRNO();
    RL_ASSERT(false);
    return;
  }

  if (!_clientAvailablilityCallback) {
    /*
     *  If the channel availability handler is not set, the server
     *  is listening for connections but won't actually do anything
     *  when one can be established. Instead of keeping clients in limbo,
     *  terminate the connection.
     */
    RL_CHECK(::close(connectionHandle));
    return;
  }

  _clientAvailablilityCallback(std::make_shared<Channel>(connectionHandle));
}

std::shared_ptr<LooperSource> Server::clientConnectionsSource() {
  if (_clientConnectionsSource.get() != nullptr) {
    return _clientConnectionsSource;
  }

  RL_ASSERT(_socketHandle != -1);

  LooperSource::IOHandlesAllocator allocator =
      [&]() { return LooperSource::Handles(_socketHandle, -1); };

  LooperSource::IOHandler readHandler = [&](LooperSource::Handle handle) {
    this->onConnectionAvailableForAccept(handle);
  };

  _clientConnectionsSource =
      std::make_shared<LooperSource>(allocator, nullptr, readHandler, nullptr);

  return _clientConnectionsSource;
}

}  // namespace rl
