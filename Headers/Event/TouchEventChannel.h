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
  using TouchEventCallback =
      std::function<void(const std::vector<TouchEvent>& touches,
                         TouchEvent::Phase phase)>;

  explicit TouchEventChannel();

  TouchEventCallback touchEventCallback() const;
  void setTouchEventCallback(TouchEventCallback callback);

  void sendTouchEvents(const std::vector<TouchEvent>& touchEvents);

 private:
  TouchEventCallback _callback;
  std::vector<TouchEvent> _pendingTouchesBegan;
  std::vector<TouchEvent> _pendingTouchesMoved;
  std::vector<TouchEvent> _pendingTouchesEnded;
  std::vector<TouchEvent> _pendingTouchesCancelled;

  void processRawTouches(Messages& messages);
  void dispatchPendingTouches();
  std::vector<TouchEvent>& bufferForPhase(TouchEvent::Phase phase);

  RL_DISALLOW_COPY_AND_ASSIGN(TouchEventChannel);
};
}  // namespace rl

#endif  // RADARLOVE_EVENT_TOUCHEVENTCHANNEL_
