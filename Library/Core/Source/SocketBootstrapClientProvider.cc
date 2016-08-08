// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "Config.h"

#if RL_CHANNELS == RL_CHANNELS_SOCKET

#include <Core/Channel.h>

#include "SocketBootstrapClientProvider.h"
#include "SocketBootstrapServerProvider.h"

namespace rl {
namespace core {

static const ClockDurationNano SocketBootstrapClientTimeout(100000000);

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

  Messages messages;
  messages.emplace_back(std::move(message));

  auto channel = SocketServer::ConnectedChannel(
      URI{SocketBootstrapServerProvider::kDefaultSocketPath});

  if (channel == nullptr) {
    RL_LOG("Could not connect to the bootstrap server.");
    return nullptr;
  }

  if (channel->sendMessages(std::move(messages),
                            SocketBootstrapClientTimeout) !=
      IOResult::Success) {
    RL_LOG("Could not send messages to the connected bootstrap server.");
    return nullptr;
  }

  auto replyMessages =
      channel->drainPendingMessages(SocketBootstrapClientTimeout);

  if (replyMessages.size() != 1) {
    RL_LOG(
        "The socket bootstrap server returned an unexpected number of "
        "replies (%zu).",
        replyMessages.size());
    return nullptr;
  }

  auto& reply = replyMessages[0];

  RawAttachment remoteChannelAttachment;
  if (!reply.decode(remoteChannelAttachment)) {
    RL_LOG(
        "Reply from remote socket bootstrap server did not contain the channel "
        "attachment.");
    return nullptr;
  }

  return std::make_shared<Channel>(std::move(remoteChannelAttachment));
}

}  // namespace core
}  // namespace rl

#endif  // RL_CHANNELS == RL_CHANNELS_SOCKET
