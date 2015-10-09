// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <Core/Config.h>

#if RL_WAITSET == RL_WAITSET_INPROCESS

#include <Core/Macros.h>
#include <Core/EventLoopSource.h>
#include <Core/Utilities.h>

#include "InProcessTrivialSource.h"
#include "InProcessTimerSource.h"

namespace rl {

void EventLoopSource::updateInWaitSetForSimpleRead(WaitSet& waitset,
                                                   bool shouldAdd) {
  RL_ASSERT_MSG(
      false,
      "In-process loop sources require an extra bit of context to signal "
      "the waitset from userspace. There can be no default simple read "
      "handler without this.");
}

std::shared_ptr<EventLoopSource> EventLoopSource::Timer(
    ClockDurationNano repeatInterval) {
  return std::make_shared<InProcessTimerSource>(repeatInterval);
}

std::shared_ptr<EventLoopSource> EventLoopSource::Trivial() {
  return std::make_shared<InProcessTrivialSource>();
}

}  // namespace rl

#endif
