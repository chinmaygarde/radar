// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef RADARLOVE_CORE_BOOTSTRAP_SERVER_H_
#define RADARLOVE_CORE_BOOTSTRAP_SERVER_H_

#include <Core/Channel.h>

namespace rl {
namespace core {

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
IOResult BootstrapServerAdvertise(const std::string& name,
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

/**
 *  Setup a globally accessible bootstrap server in this process. This is a
 *  coordinator only API.
 *
 *  @return if the global bootstrap server was successfully setup.
 */
bool BootstrapServerSetup();

/**
 *  Teardown the globally accessible bootstrap server that was previously
 *  initialized in this process. This is a coordinator only API
 *
 *  @return if the global bootstrap server was successfully torn down. Returns
 *          true if this process does not contain a functional bootstrap server.
 */
bool BootstrapServerTeardown();

}  // namespace core
}  // namespace rl

#endif  // RADARLOVE_CORE_BOOTSTRAP_SERVER_H_
