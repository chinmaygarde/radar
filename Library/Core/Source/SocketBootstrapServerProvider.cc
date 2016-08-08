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

static const ClockDurationNano SocketBootstrapServerTimeout(100000000);

const char* const SocketBootstrapServerProvider::kDefaultSocketPath =
    "/tmp/radar.bootstrap.server.sk";

SocketBootstrapServerProvider::SocketBootstrapServerProvider()
    : _isReady(false) {
  _acceptSource = _server.acceptSource(std::bind(
      &SocketBootstrapServerProvider::onAccept, this, std::placeholders::_1));

  if (_acceptSource == nullptr) {
    return;
  }

  auto loopAccess = _thread.loop();

  EventLoop* loop = loopAccess.get();

  if (!loop->addSource(_acceptSource)) {
    RL_LOG(
        "Could not add the server source to the loop setup to service "
        "bootstrap server connections.");
    return;
  }

  AutoLatch latch(1);
  loop->dispatchAsync(std::bind(&SocketBootstrapServerProvider::serverMain,
                                this, std::ref(latch)));

  _isReady = true;
}

SocketBootstrapServerProvider::~SocketBootstrapServerProvider() {
  if (!_isReady) {
    return;
  }

  _thread.loop().get()->removeSource(_acceptSource);
}

bool SocketBootstrapServerProvider::isReady() const {
  return _isReady;
}

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

  Messages messages =
      channel.drainPendingMessages(SocketBootstrapServerTimeout);

  if (messages.size() != 1) {
    RL_LOG("Received unexpected number of messages from the bootstrap client.");
    return;
  }

  auto& message = messages[0];

  std::string registrationName;

  if (!message.decode(registrationName)) {
    RL_LOG("Could not decode channel name.");
    return;
  }

  Message reply;

  RawAttachment optionalAttachment;

  if (message.decode(optionalAttachment)) {
    /*
     *  If there is an attachment, the client wants us to update a registration.
     */
    if (!reply.encode(updateRegistration(registrationName,
                                         std::move(optionalAttachment)))) {
      return;
    }
  } else {
    /*
     *  If there is no attachment, the client is querying us for a registration.
     */
    auto socketPair = acquireRegistration(registrationName);

    bool acquired = socketPair != nullptr;

    if (!reply.encode(acquired)) {
      return;
    }

    if (acquired) {
      if (!reply.encode(std::move(socketPair))) {
        return;
      }
    }
  }

  Messages replies;
  replies.emplace_back(std::move(reply));

  if (channel.sendMessages(std::move(replies), SocketBootstrapServerTimeout) !=
      IOResult::Success) {
    RL_LOG("Could not send the reply message back to the bootstrap client.");
    return;
  }
}

std::shared_ptr<SocketPair> SocketBootstrapServerProvider::acquireRegistration(
    const std::string& name) {
  if (name.size() == 0) {
    return nullptr;
  }

  return _registrations[name];
}

bool SocketBootstrapServerProvider::updateRegistration(
    const std::string& name,
    RawAttachment attachment) {
  if (name.size() == 0) {
    return false;
  }

  auto found = _registrations.find(name);

  if (found != _registrations.end()) {
    return false;
  }

  _registrations[name] = std::make_shared<SocketPair>(std::move(attachment));
  return true;
}

}  // namespace core
}  // namespace rl

#endif  // RL_CHANNELS == RL_CHANNELS_SOCKET
