// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <Core/Config.h>

#if RL_CHANNELS == RL_CHANNELS_INPROCESS

#include <Core/BootstrapServer.h>

#include <map>

namespace rl {
namespace core {

static std::map<std::string, std::shared_ptr<core::Channel>> BootstrapRegistry;

bool BootstrapServerAdvertise(const std::string& name,
                              std::shared_ptr<core::Channel> channel) {
  if (name.size() == 0 || channel == nullptr) {
    return false;
  }

  auto result = BootstrapRegistry.emplace(name, channel);

  return result.second;
}

std::shared_ptr<core::Channel> BootstrapServerAcquireAdvertised(
    const std::string& name) {
  auto prototype = BootstrapRegistry[name];

  if (!prototype) {
    return nullptr;
  }

  return std::make_shared<core::Channel>(prototype->asMessageAttachment());
}

}  // namespace core
}  // namespace rl

#endif  // RL_CHANNELS == RL_CHANNELS_INPROCESS
