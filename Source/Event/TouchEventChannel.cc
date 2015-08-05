// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "Core/Message.h"
#include "Event/TouchEventChannel.h"

namespace rl {

TouchEventChannel::TouchEventChannel() : Channel() {
}

void TouchEventChannel::sendTouchEvent(const TouchEvent&& touch) {
  sendMessage(touch.serialize());
}

}  // namespace rl
