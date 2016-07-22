// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <Core/Config.h>

#if RL_CHANNELS == RL_CHANNELS_SOCKET

#include "SocketBootstrapClientProvider.h"

namespace rl {
namespace core {

SocketBootstrapClientProvider::SocketBootstrapClientProvider() = default;

SocketBootstrapClientProvider::~SocketBootstrapClientProvider() = default;

IOResult SocketBootstrapClientProvider::doAdvertise(
    const std::string& name,
    std::shared_ptr<Channel> channel) {}

std::shared_ptr<Channel> SocketBootstrapClientProvider::doAcquire(
    const std::string& name) {}

}  // namespace core
}  // namespace rl

#endif  // RL_CHANNELS == RL_CHANNELS_SOCKET
