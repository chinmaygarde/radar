// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <Animation/SimulationGroup.h>
#include <Animation/AnimationUtilities.h>

namespace rl {
namespace animation {

double SimulationGroup::x(const core::ClockDuration& time) {
  stepIfNecessary(time);
  return currentSimulation()->x(time - currentIntervalOffset());
}

double SimulationGroup::dx(const core::ClockDuration& time) {
  stepIfNecessary(time);
  return currentSimulation()->dx(time - currentIntervalOffset());
}

bool SimulationGroup::isDone(const core::ClockDuration& time) {
  stepIfNecessary(time);
  return currentSimulation()->isDone(time - currentIntervalOffset());
}

void SimulationGroup::stepIfNecessary(const core::ClockDuration& time) {
  if (NearEqual(_lastStep.count(), time.count(), TimeTolerance)) {
    return;
  }

  _lastStep = time;
  step(time);
}

}  // namespace animation
}  // namespace rl
