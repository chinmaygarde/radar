/*
 *  This source file is part of the Radar project.
 *  Licensed under the MIT License. See LICENSE file for details.
 */

#include <Animation/AnimationUtilities.h>
#include <Animation/SpringSimulation.h>
#include <Animation/SpringSolution.h>

namespace rl {
namespace animation {

SpringSimulation::SpringSimulation(SpringDescription desc,
                                   double start,
                                   double end,
                                   double velocity)
    : _endPosition(end),
      _solution(SpringSolution::Create(desc, start - end, velocity)) {}

double SpringSimulation::x(const core::ClockDuration& time) const {
  return _endPosition + _solution->x(time);
}

double SpringSimulation::dx(const core::ClockDuration& time) const {
  return _solution->dx(time);
}

bool SpringSimulation::isDone(const core::ClockDuration& time) const {
  return NearEqual(x(time), _endPosition, DistanceTolerance) &&
         NearZero(dx(time), VelocityTolerance);
}

}  // namespace animation
}  // namespace rl
