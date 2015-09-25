// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <Animation/GravitySimulation.h>
#include <cmath>

namespace rl {

GravitySimulation::GravitySimulation(double acceleration,
                                     double distance,
                                     double endDistance,
                                     double velocity)
    : _a(acceleration), _x(distance), _v(velocity), _end(endDistance) {
}

double GravitySimulation::x(const ClockDuration& time) {
  return _x + _v * time.count() + 0.5 * _a * time.count() * time.count();
}

double GravitySimulation::dx(const ClockDuration& time) {
  return _v + time.count() * _a;
}

bool GravitySimulation::isDone(const ClockDuration& time) {
  return fabs(x(time)) >= _end;
}

}  // namespace rl
