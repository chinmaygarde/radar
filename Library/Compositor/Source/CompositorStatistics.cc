// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

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
