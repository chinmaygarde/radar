// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <Coordinator/Statistics.h>

namespace rl {
namespace coordinator {

Statistics::Statistics() {}

instrumentation::Stopwatch& Statistics::frameTimer() {
  return _frameTimer;
}

instrumentation::Stopwatch& Statistics::transactionUpdateTimer() {
  return _transactionUpdateTimer;
}

instrumentation::Stopwatch& Statistics::interpolations() {
  return _interpolations;
}

instrumentation::Counter& Statistics::interpolationsCount() {
  return _interpolationsCount;
}

instrumentation::Counter& Statistics::entityCount() {
  return _entityCount;
}

instrumentation::Counter& Statistics::primitiveCount() {
  return _primitiveCount;
}

instrumentation::Counter& Statistics::frameCount() {
  return _frameCount;
}

void Statistics::start() {
  _frameTimer.start();
}

void Statistics::stop() {
  _frameTimer.stop();

  _entityCount.reset();
  _primitiveCount.reset();
}

StatisticsFrame::StatisticsFrame(Statistics& stats) : _stats(stats) {
  stats.start();
}

StatisticsFrame::~StatisticsFrame() {
  _stats.stop();
}

}  // namespace coordinator
}  // namespace rl
