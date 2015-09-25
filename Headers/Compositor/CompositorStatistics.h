// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef RADARLOVE_COMPOSITOR_COMPOSITORSTATISTICS_H_
#define RADARLOVE_COMPOSITOR_COMPOSITORSTATISTICS_H_

#include <Core/Core.h>
#include <Instrumentation/Instrumentation.h>

namespace rl {

class CompositorStatistics {
 public:
  explicit CompositorStatistics();

  Stopwatch& frameTimer();

  Stopwatch& transactionUpdateTimer();

  Stopwatch& interpolations();

  Counter& interpolationsCount();

  Counter& entityCount();

  Counter& primitiveCount();

  Counter& frameCount();

  void start();

  void stop();

 private:
  Stopwatch _frameTimer;
  Stopwatch _transactionUpdateTimer;
  Stopwatch _interpolations;
  Counter _interpolationsCount;
  Counter _entityCount;
  Counter _primitiveCount;
  Counter _frameCount;

  RL_DISALLOW_COPY_AND_ASSIGN(CompositorStatistics);
};

class CompositorStatisticsFrame {
 public:
  CompositorStatisticsFrame(CompositorStatistics& stats);
  ~CompositorStatisticsFrame();

 private:
  CompositorStatistics& _stats;

  RL_DISALLOW_COPY_AND_ASSIGN(CompositorStatisticsFrame);
};

}  // namespace rl

#endif  // RADARLOVE_COMPOSITOR_COMPOSITORSTATISTICS_H_
