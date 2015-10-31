// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <Event/TouchEventChannel.h>

namespace rl {
namespace event {

TouchEventChannel::TouchEventChannel() : Channel() {
}

void TouchEventChannel::sendTouchEvents(
    const std::vector<TouchEvent>& touchEvents) {
  core::Messages messages;
  for (const auto& touch : touchEvents) {
    core::Message m(sizeof(TouchEvent));
    touch.serialize(m);
    messages.emplace_back(std::move(m));
  }
  bool result = sendMessages(std::move(messages));
  RL_ASSERT(result);
}

TouchEvent::PhaseMap TouchEventChannel::drainPendingTouches() {
  auto messages = drainPendingMessages();

  TouchEvent::PhaseMap results;

  if (messages.size() == 0) {
    return std::move(results);
  }

  for (auto& message : messages) {
    TouchEvent event(message);
    results[event.phase()].emplace_back(std::move(event));
  }

  return std::move(results);
}

}  // namespace event
}  // namespace rl
