// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef RADARLOVE_EVENT_TOUCHEVENTCHANNEL_
#define RADARLOVE_EVENT_TOUCHEVENTCHANNEL_

#include <Core/Channel.h>
#include <Event/TouchEvent.h>

namespace rl {
class TouchEventChannel : public Channel {
 public:
  explicit TouchEventChannel();

  void sendTouchEvents(const std::vector<TouchEvent>& touchEvents);

  TouchEvent::PhaseMap drainPendingTouches();

 private:
  std::vector<TouchEvent>& bufferForPhase(TouchEvent::Phase phase);

  RL_DISALLOW_COPY_AND_ASSIGN(TouchEventChannel);
};
}  // namespace rl

#endif  // RADARLOVE_EVENT_TOUCHEVENTCHANNEL_
