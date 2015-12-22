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

std::shared_ptr<core::Channel> Server::channelForName(const std::string& name) {
  auto found = _vendors.find(name);

  if (found == _vendors.end()) {
    /*
     *  Could not find service for the given name
     */
    return nullptr;
  }

  return found->second();
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
