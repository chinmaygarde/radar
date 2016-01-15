// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <Core/Config.h>

#if RL_CHANNELS == RL_CHANNELS_SOCKET

#include <Bootstrap/BootstrapServer.h>

namespace rl {
namespace bootstrap {

bool BootstrapServerAdvertise(const std::string& name,
                              std::shared_ptr<core::Channel> channel) {
  RL_ASSERT_MSG(false, "WIP");
  return false;
}

std::shared_ptr<core::Channel> BootstrapServerAcquireAdvertised(
    const std::string& name) {
  RL_ASSERT_MSG(false, "WIP");
  return nullptr;
}

}  // namespace bootstrap
}  // namespace rl

#endif  // RL_CHANNELS == RL_CHANNELS_SOCKET
