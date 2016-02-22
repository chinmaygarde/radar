// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef RADARLOVE_CORE_TIME_
#define RADARLOVE_CORE_TIME_

#include <chrono>

namespace rl {
namespace core {

using Clock = std::chrono::high_resolution_clock;

using ClockDurationSeconds = std::chrono::duration<double /*, std::ratio<1>*/>;
using ClockDurationNano = std::chrono::nanoseconds;
using ClockDurationMilli = std::chrono::milliseconds;
using ClockDurationMicro = std::chrono::microseconds;

using ClockPointSeconds = std::chrono::time_point<Clock, ClockDurationSeconds>;
using ClockPointNano = std::chrono::time_point<Clock, ClockDurationNano>;
using ClockPointMilli = std::chrono::time_point<Clock, ClockDurationMilli>;
using ClockPointMicro = std::chrono::time_point<Clock, ClockDurationMicro>;

using ClockDuration = ClockDurationSeconds;
using ClockPoint = ClockPointSeconds;

namespace clock {

ClockDurationSeconds LoggingClockDuration(void);

}  // namespace clock

}  // namespace core
}  // namespace rl

#endif  // RADARLOVE_CORE_TIME_
