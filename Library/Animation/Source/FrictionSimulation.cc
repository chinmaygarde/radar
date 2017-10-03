/*
 *  This source file is part of the Radar project.
 *  Licensed under the MIT License. See LICENSE file for details.
 */

#include <Animation/AnimationUtilities.h>
#include <Animation/FrictionSimulation.h>
#include <cmath>

namespace rl {
namespace animation {

FrictionSimulation::FrictionSimulation(double drag,
                                       double position,
                                       double velocity)
    : _drag(drag), _dragLog(log(drag)), _x(position), _v(velocity) {}

double FrictionSimulation::x(const core::ClockDuration& time) const {
  return _x + _v * pow(_drag, time.count()) / _dragLog - _v / _dragLog;
}

double FrictionSimulation::dx(const core::ClockDuration& time) const {
  return _v * pow(_drag, time.count());
}

bool FrictionSimulation::isDone(const core::ClockDuration& time) const {
  return fabs(dx(time)) < VelocityTolerance;
}

}  // namespace animation
}  // namespace rl
