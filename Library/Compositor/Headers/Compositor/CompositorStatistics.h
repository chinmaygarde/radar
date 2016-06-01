// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef RADARLOVE_COMPOSITOR_COMPOSITORSTATISTICS_H_
#define RADARLOVE_COMPOSITOR_COMPOSITORSTATISTICS_H_

#include <Core/Core.h>
#include <Core/Instrumentation.h>

namespace rl {
namespace compositor {

class CompositorStatistics {
 public:
  explicit CompositorStatistics();

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

#endif  // RADARLOVE_COMPOSITOR_COMPOSITORSTATISTICS_H_
