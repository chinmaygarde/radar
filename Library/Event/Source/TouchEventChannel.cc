/*
 *  This source file is part of the Radar project.
 *  Licensed under the MIT License. See LICENSE file for details.
 */

#include <Core/TraceEvent.h>
#include <Event/TouchEventChannel.h>

namespace rl {
namespace event {

TouchEventChannel::TouchEventChannel() : Channel() {}

bool TouchEventChannel::sendTouchEvents(
    const std::vector<TouchEvent>& touchEvents) {
  RL_TRACE_AUTO("SendTouchEvents");

  core::Messages messages;

  for (const auto& touch : touchEvents) {
    core::Message m;
    if (!m.encode(touch)) {
      return false;
    }
    messages.emplace_back(std::move(m));
  }

  return sendMessages(std::move(messages)) == core::IOResult::Success;
}

TouchEvent::PhaseMap TouchEventChannel::drainPendingTouches() {
  auto messages = drainPendingMessages(core::ClockDurationNano{0});

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
