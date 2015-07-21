// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef __RADARLOVE__SERVER__
#define __RADARLOVE__SERVER__

#include "Base.h"

#include <string>
#include <functional>

#include "LooperSource.h"
#include "Channel.h"

namespace rl {

class Server {
 public:
  typedef int Handle;

  typedef std::function<void(std::shared_ptr<Channel>)>
      ChannelAvailabilityCallback;

  explicit Server(std::string name);
  ~Server();

  std::shared_ptr<LooperSource> clientConnectionsSource();

  /*
   *  Server Status
   */

  bool isListening() const { return _listening; }

  void channelAvailabilityCallback(ChannelAvailabilityCallback callback) {
    _channelAvailablilityCallback = callback;
  }

  ChannelAvailabilityCallback channelAvailabilityCallback() const {
    return _channelAvailablilityCallback;
  }

 private:
  std::string _name;

  Handle _socketHandle;
  bool _listening;

  std::shared_ptr<LooperSource> _clientConnectionsSource;

  ChannelAvailabilityCallback _channelAvailablilityCallback;

  void onConnectionAvailableForAccept(Handle handle);

  DISALLOW_COPY_AND_ASSIGN(Server);
};
}

#endif /* defined(__RADARLOVE__SERVER__) */
