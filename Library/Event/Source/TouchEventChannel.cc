// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <Event/TouchEventChannel.h>
#include <Core/TraceEvent.h>

namespace rl {
namespace event {

TouchEventChannel::TouchEventChannel() : Channel() {}

void TouchEventChannel::sendTouchEvents(
    const std::vector<TouchEvent>& touchEvents) {
  RL_TRACE_AUTO("SendTouchEvents");

  core::Messages messages;

  for (const auto& touch : touchEvents) {
    core::Message m(sizeof(TouchEvent));
    m.encode(touch);
    messages.emplace_back(std::move(m));
  }

  bool result = sendMessages(std::move(messages)) == core::IOResult::Success;
  RL_ASSERT(result);
}

TouchEvent::PhaseMap TouchEventChannel::drainPendingTouches() {
  auto messages = drainPendingMessages();

  TouchEvent::PhaseMap results;

  if (messages.size() == 0) {
    return results;
  }

  for (auto& message : messages) {
    TouchEvent event(message);
    results[event.phase()].emplace_back(std::move(event));
  }

  return results;
}

}  // namespace event
}  // namespace rl
