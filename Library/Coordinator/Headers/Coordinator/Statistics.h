// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef RADARLOVE_COORDINATOR_COMPOSITORSTATISTICS_H_
#define RADARLOVE_COORDINATOR_COMPOSITORSTATISTICS_H_

#include <Core/Core.h>
#include <Core/Instrumentation.h>

namespace rl {
namespace coordinator {

class Statistics {
 public:
  explicit Statistics();

  instrumentation::Stopwatch& frameTimer();

  instrumentation::Stopwatch& transactionUpdateTimer();

  instrumentation::Stopwatch& interpolations();

  instrumentation::Counter& interpolationsCount();

  instrumentation::Counter& entityCount();

  instrumentation::Counter& primitiveCount();

  instrumentation::Counter& frameCount();

  void start();

  void stop();

 private:
  instrumentation::Stopwatch _frameTimer;
  instrumentation::Stopwatch _transactionUpdateTimer;
  instrumentation::Stopwatch _interpolations;
  instrumentation::Counter _interpolationsCount;
  instrumentation::Counter _entityCount;
  instrumentation::Counter _primitiveCount;
  instrumentation::Counter _frameCount;

  RL_DISALLOW_COPY_AND_ASSIGN(Statistics);
};

class StatisticsFrame {
 public:
  StatisticsFrame(Statistics& stats);
  ~StatisticsFrame();

 private:
  Statistics& _stats;

  RL_DISALLOW_COPY_AND_ASSIGN(StatisticsFrame);
};

}  // namespace coordinator
}  // namespace rl

#endif  // RADARLOVE_COORDINATOR_COMPOSITORSTATISTICS_H_
