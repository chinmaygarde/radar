// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <Compositor/CompositorStatistics.h>

namespace rl {

CompositorStatistics::CompositorStatistics() {
}

instrumentation::Stopwatch& CompositorStatistics::frameTimer() {
  return _frameTimer;
}

instrumentation::Stopwatch& CompositorStatistics::transactionUpdateTimer() {
  return _transactionUpdateTimer;
}

instrumentation::Stopwatch& CompositorStatistics::interpolations() {
  return _interpolations;
}

instrumentation::Counter& CompositorStatistics::interpolationsCount() {
  return _interpolationsCount;
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
