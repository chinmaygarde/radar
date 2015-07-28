// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "Animation/FrictionSimulation.h"
#include "Animation/Utilities.h"
#include <cmath>

using namespace rl;

FrictionSimulation::FrictionSimulation(double drag,
                                       double position,
                                       double velocity)
    : _drag(drag), _dragLog(log(drag)), _x(position), _v(velocity) {
}

double FrictionSimulation::x(double time) {
  return _x + _v * pow(_drag, time) / _dragLog - _v / _dragLog;
}

double FrictionSimulation::dx(double time) {
  return _v * pow(_drag, time);
}

bool FrictionSimulation::isDone(double time) {
  return fabs(dx(time)) < Animation::VelocityTolerance;
}
