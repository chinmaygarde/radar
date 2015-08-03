// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "Animation/SimulationGroup.h"

namespace rl {

double SimulationGroup::x(double time) {
  stepIfNecessary(time);
  return currentSimulation()->x(time - currentIntervalOffset());
}

double SimulationGroup::dx(double time) {
  stepIfNecessary(time);
  return currentSimulation()->dx(time - currentIntervalOffset());
}

bool SimulationGroup::isDone(double time) {
  stepIfNecessary(time);
  return currentSimulation()->isDone(time);
}

void SimulationGroup::stepIfNecessary(double time) {
  if (Animation::NearEqual(_lastStep, time, Animation::TimeTolerance)) {
    return;
  }

  _lastStep = time;
  step(time);
}

}  // namespace rl
