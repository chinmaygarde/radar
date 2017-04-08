/*
 *  This source file is part of the Radar project.
 *  Licensed under the MIT License. See LICENSE file for details.
 */

#pragma once

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
