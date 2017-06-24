/*
 *  This source file is part of the Radar project.
 *  Licensed under the MIT License. See LICENSE file for details.
 */

#include <Compositor/CompositorStatistics.h>
#include "Console.h"

namespace rl {
namespace compositor {

CompositorStatistics::CompositorStatistics() = default;

CompositorStatistics::~CompositorStatistics() = default;

instrumentation::Stopwatch& CompositorStatistics::frameTimer() {
  return _frameTimer;
}

instrumentation::Counter& CompositorStatistics::entityCount() {
  return _entityCount;
}

instrumentation::Counter& CompositorStatistics::primitiveCount() {
  return _primitiveCount;
}

instrumentation::Counter& CompositorStatistics::frameCount() {
  return _frameCount;
}

void CompositorStatistics::start() {
  _frameTimer.start();
  _frameCount.increment();
}

void CompositorStatistics::stop() {
  displayCurrentStatisticsToConsole();

  _frameTimer.stop();
  _entityCount.reset();
  _primitiveCount.reset();
}

void CompositorStatistics::displayCurrentStatisticsToConsole() const {
  auto frameMs = _frameTimer.currentLap().count() * 1e3;
  RL_CONSOLE_DISPLAY_VALUE("FPS: %.2f ms (%.0f FPS)", frameMs,
                           1000.0 / frameMs);
  RL_CONSOLE_DISPLAY_VALUE("Entities: %zu", _entityCount.count());
  RL_CONSOLE_DISPLAY_VALUE("Primitives: %zu", _primitiveCount.count());
  RL_CONSOLE_DISPLAY_VALUE("Frame Count: %zu", _frameCount.count());
}

}  // namespace compositor
}  // namespace rl
