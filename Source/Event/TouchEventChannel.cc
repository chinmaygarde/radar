// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <Event/TouchEventChannel.h>

namespace rl {

TouchEventChannel::TouchEventChannel() : Channel() {
}

void TouchEventChannel::sendTouchEvents(
    const std::vector<TouchEvent>& touchEvents) {
  Messages messages;
  for (const auto& touch : touchEvents) {
    Message m(sizeof(TouchEvent));
    touch.serialize(m);
    messages.emplace_back(std::move(m));
  }
  bool result = sendMessages(std::move(messages));
  RL_ASSERT(result);
}

std::vector<TouchEvent>& TouchEventChannel::bufferForPhase(
    TouchEvent::Phase phase) {
  switch (phase) {
    case TouchEvent::Began:
      return _pendingTouchesBegan;
    case TouchEvent::Moved:
      return _pendingTouchesMoved;
    case TouchEvent::Ended:
      return _pendingTouchesEnded;
    case TouchEvent::Cancelled:
      return _pendingTouchesCancelled;
    default:
      RL_ASSERT(false);
  }

  return _pendingTouchesCancelled;
}

void TouchEventChannel::processRawTouches(Messages& messages) {
  for (auto& message : messages) {
    TouchEvent event(message);
    bufferForPhase(event.phase()).emplace_back(std::move(event));
  }
  dispatchPendingTouches();
}

void TouchEventChannel::dispatchPendingTouches() {
  RL_ASSERT(false && "WIP");

done:
  _pendingTouchesBegan.clear();
  _pendingTouchesMoved.clear();
  _pendingTouchesEnded.clear();
  _pendingTouchesCancelled.clear();
}

}  // namespace rl
