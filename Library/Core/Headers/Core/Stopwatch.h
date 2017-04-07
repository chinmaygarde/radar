// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#pragma once

#include <Core/Macros.h>

namespace rl {
namespace instrumentation {

class Stopwatch {
 public:
  Stopwatch();

  core::ClockDuration lastLap() const;

  core::ClockDuration currentLap() const;

  void start();

  void stop();

 private:
  core::ClockPoint _startPoint;
  core::ClockDuration _lastLap;

  RL_DISALLOW_COPY_AND_ASSIGN(Stopwatch);
};

class AutoStopwatchLap {
 public:
  AutoStopwatchLap(Stopwatch& stopwatch);

  ~AutoStopwatchLap();

 private:
  Stopwatch& _stopwatch;

  RL_DISALLOW_COPY_AND_ASSIGN(AutoStopwatchLap);
};

}  // namespace instrumentation
}  // namespace rl
