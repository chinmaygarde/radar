/*
 *  This source file is part of the Radar project.
 *  Licensed under the MIT License. See LICENSE file for details.
 */

#include <Core/Stopwatch.h>
#include <TestRunner/TestRunner.h>
#include <thread>

TEST_SLOW(StopwatchTest, SimpleStopwatch) {
  rl::instrumentation::Stopwatch stopwatch;
  stopwatch.start();
  const double seconds = 0.25;
  std::this_thread::sleep_for(rl::core::ClockDurationSeconds(seconds));
  stopwatch.stop();
  ASSERT_GE(stopwatch.lastLap().count(), seconds);
}
