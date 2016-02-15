// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <Core/Config.h>

#if RL_WAITSET == RL_WAITSET_INPROCESS

#include <Core/InProcessTimerSource.h>
#include <Core/InProcessWaitSet.h>

#include <limits>

namespace rl {
namespace core {

InProcessTimerSource::InProcessTimerSource(ClockDurationNano repeatInterval)
    : EventLoopSource() {
  setHandlesProvider([repeatInterval] {
    return InProcessWaitSet::TimerHandles(repeatInterval);
  });

  setCustomWaitSetUpdateHandler(
      [&](EventLoopSource&, WaitSet& waitset, Handle, bool adding) {
        if (adding) {
          _activeWaitSets.insert(&waitset);
        } else {
          _activeWaitSets.erase(&waitset);
        }
      });
}

InProcessTimerSource::~InProcessTimerSource() {}

}  // namespace core
}  // namespace rl

#endif  // RL_WAITSET == RL_WAITSET_INPROCESS
