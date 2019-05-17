/*
 *  This source file is part of the Radar project.
 *  Licensed under the MIT License. See LICENSE file for details.
 */

#include <thread>

#include <Core/Stopwatch.h>
#include <TestRunner/TestRunner.h>

namespace rl {
namespace core {
namespace testing {

TEST_SLOW(StopwatchTest, SimpleStopwatch) {
  rl::instrumentation::Stopwatch stopwatch;
  stopwatch.start();
  const double seconds = 0.25;
  std::this_thread::sleep_for(ClockDurationSeconds(seconds));
  stopwatch.stop();
  ASSERT_GE(stopwatch.lastLap().count(), seconds);
}

TEST_SLOW(StopwatchTest, SimpleStopwatchWithLaps) {
  rl::instrumentation::Stopwatch stopwatch(3);
  ASSERT_EQ(stopwatch.samples(), 3u);

  stopwatch.start();
  std::this_thread::sleep_for(ClockDurationSeconds(.1));
  stopwatch.stop();
  ASSERT_GE(stopwatch.lastLap().count(), .1);
  ASSERT_LE(stopwatch.lastLap().count(), .15);

  stopwatch.start();
  std::this_thread::sleep_for(ClockDurationSeconds(.2));
  stopwatch.stop();
  ASSERT_GE(stopwatch.lastLap().count(), .2);
  ASSERT_LE(stopwatch.lastLap().count(), .25);

  stopwatch.start();
  std::this_thread::sleep_for(ClockDurationSeconds(.1));
  stopwatch.stop();
  ASSERT_GE(stopwatch.lastLap().count(), .1);
  ASSERT_LE(stopwatch.lastLap().count(), .15);

  stopwatch.start();
  std::this_thread::sleep_for(ClockDurationSeconds(.2));
  stopwatch.stop();
  ASSERT_GE(stopwatch.lastLap().count(), .2);
  ASSERT_LE(stopwatch.lastLap().count(), .25);

  stopwatch.start();
  std::this_thread::sleep_for(ClockDurationSeconds(.1));
  stopwatch.stop();
  ASSERT_GE(stopwatch.lastLap().count(), .1);
  ASSERT_LE(stopwatch.lastLap().count(), .15);
}

}  // namespace testing
}  // namespace core
}  // namespace rl
