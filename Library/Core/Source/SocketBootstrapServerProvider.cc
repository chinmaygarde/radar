// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "Config.h"

#if RL_CHANNELS == RL_CHANNELS_SOCKET

#include "SocketBootstrapServerProvider.h"

#include <Core/Thread.h>
#include <Core/Channel.h>

namespace rl {
namespace core {

/*
 *  All bootstrap server-client communication operates on this timeout. (100ms)
 */
static const ClockDurationNano SocketBootstrapServerTimeout(100000000);

const char* const SocketBootstrapServerProvider::kDefaultSocketPath =
    "/tmp/radar.bootstrap.server.sk";

SocketBootstrapServerProvider::SocketBootstrapServerProvider()
    : _server(std::bind(&SocketBootstrapServerProvider::onAccept,
                        this,
                        std::placeholders::_1)) {
  auto loopAccess = _thread.loop();

  EventLoop* loop = loopAccess.get();

  auto added = loop->addSource(_server.source());

  RL_ASSERT_MSG(added,
                "Must be able to add the bootstrap server source to the event "
                "loop setup to service connections.");

  AutoLatch latch(1);
  loop->dispatchAsync(std::bind(&SocketBootstrapServerProvider::serverMain,
                                this, std::ref(latch)));
}

SocketBootstrapServerProvider::~SocketBootstrapServerProvider() = default;

void SocketBootstrapServerProvider::serverMain(Latch& latch) {
  AutoCountdown latchCountdown(latch);

  thread::SetName("rl.bootstrap.server");

  if (!_server.bind(URI{kDefaultSocketPath}, true)) {
    RL_LOG(
        "Bootstrap server could not bind to '%s' to listen for IPC connections",
        kDefaultSocketPath);
    return;
  }

  if (!_server.listen(10)) {
    RL_LOG("Bootstrap server could not setup listening for IPC connections.");
    return;
  }
}

void SocketBootstrapServerProvider::onAccept(RawAttachment socket) {
  Channel channel(std::move(socket));

  Messages messages = channel.drainPendingMessages();

  if (messages.size() != 1) {
    return;
  }

  RL_WIP;
}

}  // namespace core
}  // namespace rl

#endif  // RL_CHANNELS == RL_CHANNELS_SOCKET
