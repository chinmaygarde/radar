// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "Event/TouchEventChannel.h"

#include <cassert>

namespace rl {

TouchEventChannel::TouchEventChannel() : Channel() {
  setMessagesReceivedCallback([&](Messages m) { processRawTouches(m); });
}

void TouchEventChannel::sendTouchEvents(
    const std::vector<TouchEvent>& touchEvents) {
  for (const auto& touch : touchEvents) {
    Message m(sizeof(TouchEvent));
    touch.serialize(m);
    sendMessage(m);
  }
}

void TouchEventChannel::processRawTouches(Messages& messages) {
  std::vector<TouchEvent> touches;
  for (auto& message : messages) {
    TouchEvent e;
    e.deserialize(*message);
    assert(message->size() == message->sizeRead());
  }
}

}  // namespace rl
