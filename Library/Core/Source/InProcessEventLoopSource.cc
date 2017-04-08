/*
 *  This source file is part of the Radar project.
 *  Licensed under the MIT License. See LICENSE file for details.
 */

#include "Config.h"

#if RL_WAITSET == RL_WAITSET_INPROCESS

#include <Core/EventLoopSource.h>
#include <Core/Macros.h>
#include <Core/Utilities.h>
#include "InProcessTrivialSource.h"
#include "InProcessWaitSet.h"

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
