/*
 *  This source file is part of the Radar project.
 *  Licensed under the MIT License. See LICENSE file for details.
 */

#include <Compositor/CompositorStatistics.h>

namespace rl {
namespace compositor {

CompositorStatistics::CompositorStatistics() {}

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
  _frameTimer.stop();
  _entityCount.reset();
  _primitiveCount.reset();
}

}  // namespace compositor
}  // namespace rl
