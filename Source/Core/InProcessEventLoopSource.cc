// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <Core/Config.h>

#if RL_WAITSET == RL_WAITSET_INPROCESS

#include <Core/Macros.h>
#include <Core/EventLoopSource.h>
#include <Core/Utilities.h>
#include <Core/InProcessWaitSet.h>
#include <Core/InProcessTrivialSource.h>

namespace rl {
namespace core {

void EventLoopSource::updateInWaitSetForSimpleRead(WaitSet&, bool) {}

std::shared_ptr<EventLoopSource> EventLoopSource::Timer(
    ClockDurationNano repeatInterval) {
  EventLoopSource::RWHandlesProvider provider = [repeatInterval] {
    return InProcessWaitSet::TimerHandles(repeatInterval);
  };
  /*
   *  InProcess timers require the cooperation of the platform waitset to
   *  perform their jobs. The only wait to indicate to the waitset that this
   *  source is a timer is by specifying the handles provider. Everything else
   *  is left null.
   */
  return std::make_shared<EventLoopSource>(provider, nullptr, nullptr, nullptr,
                                           nullptr);
}

std::shared_ptr<EventLoopSource> EventLoopSource::Trivial() {
  return std::make_shared<InProcessTrivialSource>();
}

}  // namespace core
}  // namespace rl

#endif  // RL_WAITSET == RL_WAITSET_INPROCESS
