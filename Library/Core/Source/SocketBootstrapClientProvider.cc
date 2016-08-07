// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "Config.h"

#if RL_CHANNELS == RL_CHANNELS_SOCKET

#include "SocketBootstrapClientProvider.h"

#include "SocketBootstrapServerProvider.h"

namespace rl {
namespace core {

SocketBootstrapClientProvider::SocketBootstrapClientProvider() = default;

SocketBootstrapClientProvider::~SocketBootstrapClientProvider() = default;

IOResult SocketBootstrapClientProvider::doAdvertise(
    const std::string& name,
    std::shared_ptr<Channel> channel) {
  RL_WIP;
  return IOResult::Failure;
}

std::shared_ptr<Channel> SocketBootstrapClientProvider::doAcquire(
    const std::string& name) {
  Message message;

  if (!message.encode(name)) {
    return nullptr;
  }

  URI serverURI(SocketBootstrapServerProvider::kDefaultSocketPath);
  SocketServer externalServer;

  if (!externalServer.connect(std::move(serverURI))) {
    RL_LOG("Could not connect to the bootstrap server.");
    return nullptr;
  }

  return nullptr;
}

}  // namespace core
}  // namespace rl

#endif  // RL_CHANNELS == RL_CHANNELS_SOCKET
