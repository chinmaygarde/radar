// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef RADARLOVE_BOOTSTRAP_SERVER_H_
#define RADARLOVE_BOOTSTRAP_SERVER_H_

#include <Core/Core.h>

#include <map>

namespace rl {
namespace bootstrap {

/**
 *  Advertises the given channel with the bootstrap server. The bootstrap server
 *  is unique to the device and common service vendors use it to advertise
 *  their services and connect to interested clients.
 *
 *  @param name    the name of the service. Must be unique to in the system
 *  @param channel the channel on which to service client requests on
 *
 *  @return if the advertisement was successful
 */
bool BootstrapServerAdvertise(const std::string& name,
                              std::shared_ptr<core::Channel> channel);

/**
 *  Get a reference to a channel that was previously advertised as being a
 *  service provider for the given name
 *
 *  @param name the advertised name of the service vendor
 *
 *  @return a reference to the advertised channel (if one is present), or nil.
 */
std::shared_ptr<core::Channel> BootstrapServerAcquireAdvertised(
    const std::string& name);

}  // namespace bootstrap
}  // namespace rl

#endif  // RADARLOVE_BOOTSTRAP_SERVER_H_
