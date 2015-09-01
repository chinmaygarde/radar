// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <Compositor/CompositorStatistics.h>

namespace rl {

CompositorStatistics::CompositorStatistics()
    : _frameTimer(),
      _transactionUpdateTimer(),
      _entityCount(),
      _primitiveCount(),
      _frameCount() {
}

Stopwatch& CompositorStatistics::frameTimer() {
  return _frameTimer;
}

Stopwatch& CompositorStatistics::transactionUpdateTimer() {
  return _transactionUpdateTimer;
}

Stopwatch& CompositorStatistics::interpolations() {
  return _interpolations;
}

Counter& CompositorStatistics::entityCount() {
  return _entityCount;
}

Counter& CompositorStatistics::primitiveCount() {
  return _primitiveCount;
}

Counter& CompositorStatistics::frameCount() {
  return _frameCount;
}

void CompositorStatistics::start() {
  _frameTimer.start();
}

void CompositorStatistics::stop() {
  _frameTimer.stop();

  _entityCount.reset();
  _primitiveCount.reset();
}

CompositorStatisticsFrame::CompositorStatisticsFrame(
    CompositorStatistics& stats)
    : _stats(stats) {
  stats.start();
}

CompositorStatisticsFrame::~CompositorStatisticsFrame() {
  _stats.stop();
}

}  // namespace rl
