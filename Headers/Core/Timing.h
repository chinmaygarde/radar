// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef RADARLOVE_CORE_TIME_
#define RADARLOVE_CORE_TIME_

#include <chrono>

namespace rl {
namespace core {

using Clock = std::chrono::high_resolution_clock;
using ClockDuration = std::chrono::duration<double /*, std::ratio<1>*/>;

using ClockDurationSeconds = ClockDuration;
using ClockDurationNano = std::chrono::nanoseconds;
using ClockDurationMilli = std::chrono::milliseconds;
using ClockDurationMicro = std::chrono::microseconds;

using ClockPoint = std::chrono::time_point<Clock, ClockDuration>;

static const ClockDurationMilli ClockDurationGod = ClockDurationMilli(16);

namespace clock {

ClockDurationSeconds LoggingClockDuration(void);

}  // namespace clock

}  // namespace core
}  // namespace rl

#endif  // RADARLOVE_CORE_TIME_
