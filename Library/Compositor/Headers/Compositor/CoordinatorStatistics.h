// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef RADARLOVE_COMPOSITOR_COORDINATORSTATISTICS_H_
#define RADARLOVE_COMPOSITOR_COORDINATORSTATISTICS_H_

#include <Core/Core.h>
#include <Core/Instrumentation.h>

namespace rl {
namespace compositor {

class CoordinatorStatistics {
 public:
  explicit CoordinatorStatistics();

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

  RL_DISALLOW_COPY_AND_ASSIGN(CoordinatorStatistics);
};

class StatisticsFrame {
 public:
  StatisticsFrame(CoordinatorStatistics& stats);
  ~StatisticsFrame();

 private:
  CoordinatorStatistics& _stats;

  RL_DISALLOW_COPY_AND_ASSIGN(StatisticsFrame);
};

}  // namespace compositor
}  // namespace rl

#endif  // RADARLOVE_COMPOSITOR_COORDINATORSTATISTICS_H_
