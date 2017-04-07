// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#pragma once

#include <Core/Channel.h>

namespace rl {
namespace core {

class BootstrapServerProvider;

class BootstrapServer {
 public:
  /**
   *  Advertises the given channel with the bootstrap server. The bootstrap
   *  server is unique to the device and common service vendors use it to
   *  advertise their services and connect to interested clients.
   *
   *  @param name    the name of the service. Must be unique to in the system
   *  @param channel the channel on which to service client requests on
   *
   *  @return if the advertisement was successful
   */
  static IOResult Advertise(const std::string& name,
                            std::shared_ptr<Channel> channel);

  /**
   *  Get a reference to a channel that was previously advertised as being a
   *  service provider for the given name
   *
   *  @param name the advertised name of the service vendor
   *
   *  @return a reference to the advertised channel (if one is present), or nil.
   */
  static std::shared_ptr<Channel> Acquire(const std::string& name);

  /**
   *  Setup a globally accessible bootstrap server hosted by this process.
   */
  BootstrapServer();

  /**
   *  Teardown the globally accessible bootstrap server that was hosted by this
   *  this process.
   */
  ~BootstrapServer();

 private:
  std::unique_ptr<BootstrapServerProvider> _provider;

  RL_DISALLOW_COPY_AND_ASSIGN(BootstrapServer);
};

}  // namespace core
}  // namespace rl
