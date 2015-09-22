// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <Event/TouchEventChannel.h>

namespace rl {

TouchEventChannel::TouchEventChannel() : Channel() {
  setMessagesReceivedCallback([&](Messages m) { processRawTouches(m); });
}

TouchEventChannel::TouchEventCallback TouchEventChannel::touchEventCallback()
    const {
  return _callback;
}

void TouchEventChannel::setTouchEventCallback(TouchEventCallback callback) {
  _callback = callback;
}

void TouchEventChannel::sendTouchEvents(
    const std::vector<TouchEvent>& touchEvents) {
  Messages messages;
  for (const auto& touch : touchEvents) {
    Message m(sizeof(TouchEvent));
    touch.serialize(m);
    messages.emplace_back(std::move(m));
  }
  sendMessages(std::move(messages));
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

  /*
   *  TODO: More thought must be put into discarding un-handled touches and
   *  and deciding how and when to make the decision to dispatch
   */
  dispatchPendingTouches();
}

void TouchEventChannel::dispatchPendingTouches() {
  if (!_callback) {
    goto done;
  }

  if (_pendingTouchesBegan.size() > 0) {
    _callback(_pendingTouchesBegan, TouchEvent::Began);
  }

  if (_pendingTouchesMoved.size() > 0) {
    _callback(_pendingTouchesMoved, TouchEvent::Moved);
  }

  if (_pendingTouchesEnded.size() > 0) {
    _callback(_pendingTouchesEnded, TouchEvent::Ended);
  }

  if (_pendingTouchesCancelled.size() > 0) {
    _callback(_pendingTouchesCancelled, TouchEvent::Cancelled);
  }

done:
  _pendingTouchesBegan.clear();
  _pendingTouchesMoved.clear();
  _pendingTouchesEnded.clear();
  _pendingTouchesCancelled.clear();
}

}  // namespace rl
