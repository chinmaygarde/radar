// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "BootstrapClient.h"

#include "SocketBootstrapClientProvider.h"
#include "MachBootstrapClientProvider.h"
#include "InProcessBootstrapClientProvider.h"

namespace rl {
namespace core {

using PlatformBootstrapClientProvider =
#if RL_CHANNELS == RL_CHANNELS_MACH
    MachBootstrapClientProvider
#elif RL_CHANNELS == RL_CHANNELS_SOCKET
    SocketBootstrapClientProvider
#elif RL_CHANNELS == RL_CHANNELS_INPROCESS
    InProcessBootstrapClientProvider
#else
#error Unknown Channels Implementation
#endif
    ;

BootstrapClient::BootstrapClient()
    : _provider(core::make_unique<PlatformBootstrapClientProvider>()) {}

BootstrapClient::~BootstrapClient() = default;

IOResult BootstrapClient::advertise(const std::string& name,
                                    std::shared_ptr<Channel> channel) {
  if (name.size() == 0) {
    return IOResult::Failure;
  }

  if (!channel) {
    return IOResult::Failure;
  }

  return _provider->doAdvertise(name, std::move(channel));
}

std::shared_ptr<Channel> BootstrapClient::acquire(const std::string& name) {
  if (name.size() == 0) {
    return nullptr;
  }

  return _provider->doAcquire(name);
}

}  // namespace core
}  // namespace rl
