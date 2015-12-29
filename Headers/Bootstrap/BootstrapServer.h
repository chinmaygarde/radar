// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef RADARLOVE_BOOTSTRAP_SERVER_H_
#define RADARLOVE_BOOTSTRAP_SERVER_H_

#include <Core/Core.h>

#include <map>

namespace rl {
namespace bootstrap {

bool BootstrapServerAdvertise(const std::string& name,
                              std::shared_ptr<core::Channel>
                                  channel);

bool BoostrapServerStopAdvertising(std::shared_ptr<core::Channel> channel);

std::shared_ptr<core::Channel> BootstrapServerAcquireAdvertised(
    const std::string& name);

}  // namespace bootstrap
}  // namespace rl

#endif  // RADARLOVE_BOOTSTRAP_SERVER_H_
