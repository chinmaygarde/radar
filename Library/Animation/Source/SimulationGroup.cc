/*
 *  This source file is part of the Radar project.
 *  Licensed under the MIT License. See LICENSE file for details.
 */

#include <Animation/AnimationUtilities.h>
#include <Animation/SimulationGroup.h>

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
