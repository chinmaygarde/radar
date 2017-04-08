/*
 *  This source file is part of the Radar project.
 *  Licensed under the MIT License. See LICENSE file for details.
 */

#include <Core/Stopwatch.h>

namespace rl {
namespace instrumentation {

Stopwatch::Stopwatch() : _startPoint(core::ClockPoint::min()), _lastLap(0) {}

void Stopwatch::start() {
  _startPoint = core::Clock::now();
}

void Stopwatch::stop() {
  _lastLap = core::Clock::now() - _startPoint;
}

core::ClockDuration Stopwatch::currentLap() const {
  return core::Clock::now() - _startPoint;
}

core::ClockDuration Stopwatch::lastLap() const {
  return _lastLap;
}

AutoStopwatchLap::AutoStopwatchLap(Stopwatch& stopwatch)
    : _stopwatch(stopwatch) {
  _stopwatch.start();
}

AutoStopwatchLap::~AutoStopwatchLap() {
  _stopwatch.stop();
}

}  // namespace instrumentation
}  // namespace rl
