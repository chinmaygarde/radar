// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef __RADARLOVE_EVENT_TOUCHEVENTCHANNEL__
#define __RADARLOVE_EVENT_TOUCHEVENTCHANNEL__

#include "Core/Channel.h"
#include "Event/TouchEvent.h"

namespace rl {
class TouchEventChannel : public Channel {
 public:
  explicit TouchEventChannel();

  void sendTouchEvents(const std::vector<TouchEvent>&& touchEvents);

 private:
  DISALLOW_COPY_AND_ASSIGN(TouchEventChannel);
  void processRawTouches(Messages& messages);
};
}  // namespace rl

#endif /* defined(__RADARLOVE_EVENT_TOUCHEVENTCHANNEL__) */
