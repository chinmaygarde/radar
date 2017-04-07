// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <Core/BootstrapServer.h>
#include "BootstrapClient.h"
#include "InProcessBootstrapServerProvider.h"
#include "MachBootstrapServerProvider.h"
#include "SocketBootstrapServerProvider.h"

namespace rl {
namespace core {

using PlatformBootstrapServerProvider =
#if RL_CHANNELS == RL_CHANNELS_MACH
    MachBootstrapServerProvider
#elif RL_CHANNELS == RL_CHANNELS_SOCKET
    SocketBootstrapServerProvider
#elif RL_CHANNELS == RL_CHANNELS_INPROCESS
    InProcessBootstrapServerProvider
#else
#error Unknown Channels Implementation
#endif
    ;

BootstrapServer::BootstrapServer()
    : _provider(std::make_unique<PlatformBootstrapServerProvider>()) {}

BootstrapServer::~BootstrapServer() = default;

IOResult BootstrapServer::Advertise(const std::string& name,
                                    std::shared_ptr<Channel> channel) {
  BootstrapClient client;
  return client.advertise(name, channel);
}

std::shared_ptr<Channel> BootstrapServer::Acquire(const std::string& name) {
  BootstrapClient client;
  return client.acquire(name);
}

}  // namespace core
}  // namespace rl
