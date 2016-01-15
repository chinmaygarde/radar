// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <Instrumentation/Stopwatch.h>

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
