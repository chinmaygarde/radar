/*
 *  This source file is part of the Radar project.
 *  Licensed under the MIT License. See LICENSE file for details.
 */

#pragma once

#include <Core/Macros.h>
#include <vector>

namespace rl {
namespace instrumentation {

class Stopwatch {
 public:
  Stopwatch(size_t samples = 1);

  core::ClockDuration lastLap() const;

  core::ClockDuration lapDuration(size_t lap) const;

  core::ClockDuration currentLap() const;

  size_t samples() const;

  void start();

  void stop();

 private:
  std::vector<core::ClockDuration> _laps;
  size_t _currentLapIndex;
  core::ClockPoint _currentLapStartPoint;

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
