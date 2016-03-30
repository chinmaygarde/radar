// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <Core/Config.h>

#if RL_CHANNELS == RL_CHANNELS_INPROCESS

#include <Core/BootstrapServer.h>

#include <map>

namespace rl {
namespace core {

static std::mutex RegistryMutex;
static std::map<std::string, std::weak_ptr<core::Channel>> BootstrapRegistry;

IOResult BootstrapServerAdvertise(const std::string& name,
                                  std::shared_ptr<core::Channel> channel) {
  if (name.size() == 0 || channel == nullptr) {
    return IOResult::Failure;
  }

  std::lock_guard<std::mutex> lock(RegistryMutex);

  auto found = BootstrapRegistry.find(name);

  /*
   *  Make sure an entry does not already exist. It it does, the channel for
   *  that should have already expired. If not, a valid channel registration is
   *  is present and this new registration should fail.
   */
  if (found != BootstrapRegistry.end() && found->second.lock()) {
    return IOResult::Failure;
  }

  BootstrapRegistry[name] = channel;
  return IOResult::Success;
}

std::shared_ptr<core::Channel> BootstrapServerAcquireAdvertised(
    const std::string& name) {
  std::lock_guard<std::mutex> lock(RegistryMutex);

  auto found = BootstrapRegistry.find(name);

  if (found == BootstrapRegistry.end()) {
    return nullptr;
  }

  if (auto prototype = found->second.lock()) {
    return std::make_shared<core::Channel>(prototype->asMessageAttachment());
  } else {
    /*
     *  Between the last time the bootstrap registry was accessed and now,
     *  the channel prototype for this name went away. Get rid of then entry
     *  in our collection. Book-keeping.
     */
    BootstrapRegistry.erase(found);
  }

  return nullptr;
}

bool BootstrapServerSetup() {
  // Nothing to do since everything is in process.
  return true;
}

bool BootstrapServerTeardown() {
  BootstrapRegistry.clear();
  return true;
}

}  // namespace core
}  // namespace rl

#endif  // RL_CHANNELS == RL_CHANNELS_INPROCESS
