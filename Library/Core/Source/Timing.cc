/*
 *  This source file is part of the Radar project.
 *  Licensed under the MIT License. See LICENSE file for details.
 */

#include <Core/Timing.h>

namespace rl {
namespace core {
namespace clock {

core::ClockDurationSeconds LoggingClockDuration(void) {
  static core::ClockPoint point;
  static bool initialized = false;
  if (!initialized) {
    point = core::Clock::now();
    initialized = true;
  }
  return core::Clock::now() - point;
}

}  // namespace clock
}  // namespace core
}  // namespace rl
