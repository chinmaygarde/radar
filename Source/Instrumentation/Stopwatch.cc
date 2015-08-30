// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <Instrumentation/Stopwatch.h>

namespace rl {

Stopwatch::Stopwatch() : _start(0), _lastLap(0) {
}

void Stopwatch::start() {
  _start = Time::Current();
}

void Stopwatch::stop() {
  _lastLap = Time::Current() - _start;
}

std::chrono::nanoseconds Stopwatch::currentLap() const {
  return Time::Current() - _start;
}

std::chrono::nanoseconds Stopwatch::lastLap() const {
  return _lastLap;
}

AutoStopwatchLap::AutoStopwatchLap(Stopwatch& stopwatch)
    : _stopwatch(stopwatch) {
  _stopwatch.start();
}

AutoStopwatchLap::~AutoStopwatchLap() {
  _stopwatch.stop();
}

}  // namespace rl
