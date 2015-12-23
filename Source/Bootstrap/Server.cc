// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <Bootstrap/Server.h>

namespace rl {
namespace bootstrap {

static std::shared_ptr<Server> _GlobalServer;

Server::Server() {}

Server& Server::Acquire() {
  RL_ASSERT_MSG(_GlobalServer != nullptr,
                "The global bootstrap server must be setup");
  return *_GlobalServer;
}

std::shared_ptr<Server> Server::Setup() {
  if (_GlobalServer != nullptr) {
    return _GlobalServer;
  }

  _GlobalServer = std::shared_ptr<Server>(new Server());
  return _GlobalServer;
}

void Server::channelForName(const std::string& name,
                            ChannelResolutionCallback callback) {
  auto found = _vendors.find(name);

  if (found == _vendors.end()) {
    /*
     *  Could not find service for the given name
     */
    return callback(nullptr);
  }

  /*
   *  Create an alias for the channel provided by the vendor
   */
  auto sourceChannel = found->second();

  if (sourceChannel == nullptr) {
    callback(nullptr);
    return;
  }

  /*
   *  WIP: In multiprocessing scenarios, the message attachment used directly
   *       below will be sent over the process boundary
   */

  auto aliased =
      std::make_shared<core::Channel>(sourceChannel->asMessageAttachment());

  callback(aliased);
}

bool Server::setVendorForName(ChannelVendor vendor, const std::string& name) {
  if (vendor == nullptr || name.length() == 0) {
    return false;
  }

  auto result = _vendors.insert({name, vendor});
  return result.second;
}

}  // namespace bootstrap
}  // namespace rl
