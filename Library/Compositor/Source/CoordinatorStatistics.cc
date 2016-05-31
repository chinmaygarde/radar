// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <Compositor/CoordinatorStatistics.h>

namespace rl {
namespace compositor {

CoordinatorStatistics::CoordinatorStatistics() {}

instrumentation::Stopwatch& CoordinatorStatistics::frameTimer() {
  return _frameTimer;
}

instrumentation::Counter& CoordinatorStatistics::entityCount() {
  return _entityCount;
}

instrumentation::Counter& CoordinatorStatistics::primitiveCount() {
  return _primitiveCount;
}

instrumentation::Counter& CoordinatorStatistics::frameCount() {
  return _frameCount;
}

void CoordinatorStatistics::start() {
  _frameTimer.start();
}

void CoordinatorStatistics::stop() {
  _frameTimer.stop();

  _entityCount.reset();
  _primitiveCount.reset();
}

StatisticsFrame::StatisticsFrame(CoordinatorStatistics& stats) : _stats(stats) {
  stats.start();
}

StatisticsFrame::~StatisticsFrame() {
  _stats.stop();
}

}  // namespace compositor
}  // namespace rl
