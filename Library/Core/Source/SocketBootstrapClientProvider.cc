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
  return bootstrapServerConnect(name, channel->attachment()).isValid()
             ? IOResult::Success
             : IOResult::Failure;
}

std::shared_ptr<Channel> SocketBootstrapClientProvider::doAcquire(
    const std::string& name) {
  Message reply = bootstrapServerConnect(name);

  if (!reply.isValid()) {
    return nullptr;
  }

  RawAttachment remoteChannelAttachment;

  if (!reply.decode(remoteChannelAttachment)) {
    RL_LOG(
        "Reply from remote socket bootstrap server did not contain the channel "
        "attachment.");
    return nullptr;
  }

  return std::make_shared<Channel>(std::move(remoteChannelAttachment));
}

Message SocketBootstrapClientProvider::bootstrapServerConnect(
    const std::string& name) {
  return bootstrapServerConnect(name, nullptr);
}

Message SocketBootstrapClientProvider::bootstrapServerConnect(
    const std::string& name,
    AttachmentRef attachment) {
  /*
   *  =========================================================================
   *  Construct the request message (with optional attachment).
   *  ==========================================================================
   */
  Message requestMessage;

  if (!requestMessage.encode(name)) {
    return {};
  }

  if (attachment != nullptr) {
    if (!requestMessage.encode(attachment)) {
      return {};
    }
  }

  Messages requestMessages;
  requestMessages.emplace_back(std::move(requestMessage));

  /*
   *  =========================================================================
   *  Acquire a connection to the remote bootstrap server.
   *  ==========================================================================
   */
  auto channel = SocketServer::ConnectedChannel(
      URI{SocketBootstrapServerProvider::kDefaultSocketPath});

  if (channel == nullptr) {
    RL_LOG("Could not connect to the bootstrap server.");
    return {};
  }

  /*
   *  =========================================================================
   *  Send the request message to the remote bootstrap server.
   *  ==========================================================================
   */
  if (channel->sendMessages(std::move(requestMessages),
                            SocketBootstrapClientTimeout) !=
      IOResult::Success) {
    RL_LOG("Could not send messages to the connected bootstrap server.");
    return {};
  }

  /*
   *  =========================================================================
   *  Await a response from the remote bootstrap server.
   *  ==========================================================================
   */
  auto replyMessages =
      channel->drainPendingMessages(SocketBootstrapClientTimeout, 1);

  if (replyMessages.size() != 1) {
    RL_LOG(
        "The socket bootstrap server returned an unexpected number of "
        "replies (%zu).",
        replyMessages.size());
    return {};
  }

  /*
   *  =========================================================================
   *  Check if the response was valid.
   *  ==========================================================================
   */

  auto& reply = replyMessages[0];

  bool querySuccessful = false;

  if (!reply.decode(querySuccessful, nullptr)) {
    RL_LOG("Could not determine if query was successful.");
    return {};
  }

  if (!querySuccessful) {
    /*
     *  The remote end did not find a valid channel registration for this
     *  name in its registry.
     */
    return {};
  }

  return std::move(reply);
}

}  // namespace core
}  // namespace rl

#endif  // RL_CHANNELS == RL_CHANNELS_SOCKET
