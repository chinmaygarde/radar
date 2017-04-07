// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#pragma once

#include <Core/Channel.h>
#include <Event/TouchEvent.h>

namespace rl {
namespace event {

class TouchEventChannel : public core::Channel {
 public:
  TouchEventChannel();

  bool sendTouchEvents(const std::vector<TouchEvent>& touchEvents);

  TouchEvent::PhaseMap drainPendingTouches();

 private:
  std::vector<TouchEvent>& bufferForPhase(TouchEvent::Phase phase);

  RL_DISALLOW_COPY_AND_ASSIGN(TouchEventChannel);
};

}  // namespace event
}  // namespace rl
