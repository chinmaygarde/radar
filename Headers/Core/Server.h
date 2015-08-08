// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef __RADARLOVE__SERVER__
#define __RADARLOVE__SERVER__

#include <Core/Macros.h>

#include <string>
#include <functional>

#include <Core/LooperSource.h>
#include <Core/Channel.h>

namespace rl {

/**
 *  A server is a standalone named channel endpoint.
 */
class Server {
 public:
  using Handle = int;
  using ClientAvailabilityCallback =
      std::function<void(std::shared_ptr<Channel>)>;

#pragma mark - Creating a Server
  /**
   *  Create a named server
   *
   *  @param name the name of the server
   */
  explicit Server(std::string name);

  ~Server();

#pragma mark - Listening for clients connections on the server

  /**
   *  Get the looper source that is signalled when new client connections are
   *  available on the server. Before the client connections source is scheduled
   *  an active looper however, the `clientAvailabilityCallback` must be set so
   *  that client channel connections may be accepted.
   *
   *  @return the client connections looper source
   */
  std::shared_ptr<LooperSource> clientConnectionsSource();

  void clientAvailabilityCallback(ClientAvailabilityCallback callback) {
    _clientAvailablilityCallback = callback;
  }

  ClientAvailabilityCallback channelAvailabilityCallback() const {
    return _clientAvailablilityCallback;
  }

#pragma mark - Misc.

  /**
   *  Get if the server is listening for client connections
   *
   *  @return is the server listening for client connections
   */
  bool isListening() const { return _listening; }

 private:
  std::string _name;
  Handle _socketHandle;
  bool _listening;
  std::shared_ptr<LooperSource> _clientConnectionsSource;
  ClientAvailabilityCallback _clientAvailablilityCallback;

  void onConnectionAvailableForAccept(Handle handle);

  DISALLOW_COPY_AND_ASSIGN(Server);
};
}

#endif /* defined(__RADARLOVE__SERVER__) */
