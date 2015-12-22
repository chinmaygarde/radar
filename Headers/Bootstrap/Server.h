// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef RADARLOVE_BOOTSTRAP_SERVER_H_
#define RADARLOVE_BOOTSTRAP_SERVER_H_

#include <Core/Core.h>

#include <map>

namespace rl {
namespace bootstrap {

class Server {
 public:
  using ChannelVendor = std::function<std::shared_ptr<core::Channel>(void)>;

  static Server& Acquire();

  static std::shared_ptr<Server> Setup();

  std::shared_ptr<core::Channel> channelForName(const std::string& name);

  bool setVendorForName(ChannelVendor vendor, const std::string& name);

 private:
  std::map<std::string, ChannelVendor> _vendors;

  Server();

  RL_DISALLOW_COPY_AND_ASSIGN(Server);
};

}  // namespace bootstrap
}  // namespace rl

#endif  // RADARLOVE_BOOTSTRAP_SERVER_H_
