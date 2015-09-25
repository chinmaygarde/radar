// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef RADARLOVE_INSTRUMENTATION_STOPWATCH_H_
#define RADARLOVE_INSTRUMENTATION_STOPWATCH_H_

#include <Core/Core.h>

namespace rl {

class Stopwatch {
 public:
  explicit Stopwatch();

  ClockDuration lastLap() const;
  ClockDuration currentLap() const;

  void start();
  void stop();

 private:
  ClockPoint _startPoint;
  ClockDuration _lastLap;

  RL_DISALLOW_COPY_AND_ASSIGN(Stopwatch);
};

class AutoStopwatchLap {
 public:
  explicit AutoStopwatchLap(Stopwatch& stopwatch);
  ~AutoStopwatchLap();

 private:
  Stopwatch& _stopwatch;

  RL_DISALLOW_COPY_AND_ASSIGN(AutoStopwatchLap);
};

}  // namespace rl

#endif  // RADARLOVE_INSTRUMENTATION_STOPWATCH_H_
