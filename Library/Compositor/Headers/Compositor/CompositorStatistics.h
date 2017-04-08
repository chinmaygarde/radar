/*
 *  This source file is part of the Radar project.
 *  Licensed under the MIT License. See LICENSE file for details.
 */

#pragma once

#include <Core/Instrumentation.h>
#include <Core/Macros.h>

namespace rl {
namespace compositor {

class CompositorStatistics {
 public:
  CompositorStatistics();

  instrumentation::Stopwatch& frameTimer();

  instrumentation::Counter& entityCount();

  instrumentation::Counter& primitiveCount();

  instrumentation::Counter& frameCount();

  void start();

  void stop();

 private:
  instrumentation::Stopwatch _frameTimer;
  instrumentation::Counter _entityCount;
  instrumentation::Counter _primitiveCount;
  instrumentation::Counter _frameCount;

  RL_DISALLOW_COPY_AND_ASSIGN(CompositorStatistics);
};

}  // namespace compositor
}  // namespace rl
