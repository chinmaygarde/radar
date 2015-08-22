// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <Compositor/CompositorStatistics.h>

namespace rl {

CompositorStatistics::CompositorStatistics()
    : _frameTimer(), _entityCount(), _primitiveCount() {
}

Stopwatch& CompositorStatistics::frameTimer() {
  return _frameTimer;
}

Counter& CompositorStatistics::entityCount() {
  return _entityCount;
}

Counter& CompositorStatistics::primitiveCount() {
  return _primitiveCount;
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
