// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "Animation/SpringSimulation.h"
#include "Animation/Utilities.h"
#include "Animation/SpringSolution.h"

namespace rl {

SpringSimulation::SpringSimulation(SpringDescription desc,
                                   double start,
                                   double end,
                                   double velocity)
    : _endPosition(end),
      _solution(SpringSolution::Create(desc, start - end, velocity)) {
}

double SpringSimulation::x(double time) {
  return _endPosition + _solution->x(time);
}

double SpringSimulation::dx(double time) {
  return _solution->dx(time);
}

bool SpringSimulation::isDone(double time) {
  return Animation::NearEqual(x(time), _endPosition,
                              Animation::DistanceTolerance) &&
         Animation::NearZero(dx(time), Animation::VelocityTolerance);
}

}  // namespace rl
