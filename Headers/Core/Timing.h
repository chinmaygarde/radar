// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef __RADARLOVE_CORE_TIME__
#define __RADARLOVE_CORE_TIME__

#include <chrono>

namespace rl {

using Clock = std::chrono::high_resolution_clock;
using ClockDuration = std::chrono::duration<double /*, std::ratio<1>*/>;

using ClockDurationSeconds = ClockDuration;
using ClockDurationNano = std::chrono::nanoseconds;
using ClockDurationMilli = std::chrono::milliseconds;
using ClockDurationMicro = std::chrono::microseconds;

using ClockPoint = std::chrono::time_point<Clock, ClockDuration>;

static const ClockDurationMilli ClockDurationGod = ClockDurationMilli(16);

namespace clock {
/**
 *  Get the absolute boot time used for logging purposes. Useless
 *  for any other purpose
 *
 *  @return the logging boot time.
 */
ClockPoint LoggingBootTime(void);

inline ClockDuration ToSeconds(ClockPoint point) {
  return std::chrono::duration_cast<ClockDuration>(point.time_since_epoch());
}

}  // namespace clock
}  // namespace rl

#endif  // __RADARLOVE_CORE_TIME__
