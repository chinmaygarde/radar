// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "InProcessTimerSource.h"
#include "InProcessWaitSet.h"

#include <limits>

namespace rl {

InProcessTimerSource::InProcessTimerSource(
    std::chrono::nanoseconds repeatInterval)
    : EventLoopSource() {
  setHandlesProvider([repeatInterval] {
    return InProcessWaitSet::TimerHandles(repeatInterval);
  });

  setCustomWaitSetUpdateHandler([&](EventLoopSource& source, WaitSet& waitset,
                                    Handle readHandle, bool adding) {
    if (adding) {
      _activeWaitSets.insert(&waitset);
    } else {
      _activeWaitSets.erase(&waitset);
    }
  });
}

InProcessTimerSource::~InProcessTimerSource() {
}

}  // namespace rl
