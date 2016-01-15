// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <Animation/FrictionSimulation.h>
#include <Animation/Utilities.h>
#include <cmath>

namespace rl {
namespace animation {

FrictionSimulation::FrictionSimulation(double drag,
                                       double position,
                                       double velocity)
    : _drag(drag), _dragLog(log(drag)), _x(position), _v(velocity) {}

double FrictionSimulation::x(const core::ClockDuration& time) {
  return _x + _v * pow(_drag, time.count()) / _dragLog - _v / _dragLog;
}

double FrictionSimulation::dx(const core::ClockDuration& time) {
  return _v * pow(_drag, time.count());
}

bool FrictionSimulation::isDone(const core::ClockDuration& time) {
  return fabs(dx(time)) < VelocityTolerance;
}

}  // namespace animation
}  // namespace rl
