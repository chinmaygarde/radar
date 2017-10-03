/*
 *  This source file is part of the Radar project.
 *  Licensed under the MIT License. See LICENSE file for details.
 */

#include <Animation/GravitySimulation.h>
#include <cmath>

namespace rl {
namespace animation {

GravitySimulation::GravitySimulation(double acceleration,
                                     double distance,
                                     double endDistance,
                                     double velocity)
    : _a(acceleration), _x(distance), _v(velocity), _end(endDistance) {}

double GravitySimulation::x(const core::ClockDuration& time) const {
  return _x + _v * time.count() + 0.5 * _a * time.count() * time.count();
}

double GravitySimulation::dx(const core::ClockDuration& time) const {
  return _v + time.count() * _a;
}

bool GravitySimulation::isDone(const core::ClockDuration& time) const {
  return fabs(x(time)) >= _end;
}

}  // namespace animation
}  // namespace rl
