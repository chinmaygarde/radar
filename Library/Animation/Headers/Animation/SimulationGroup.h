/*
 *  This source file is part of the Radar project.
 *  Licensed under the MIT License. See LICENSE file for details.
 */

#pragma once

#include <Animation/Simulation.h>

namespace rl {
namespace animation {

/*
 *  The abstract base class for all composite simulations. Concrete subclasses
 *  must implement the appropriate methods to select the appropriate simulation
 *  at a given time interval. The simulation group takes care to call the `step`
 *  method at appropriate intervals. If more fine grained control over the the
 *  step is necessary, subclasses may override `Simulatable` methods.
 */
class SimulationGroup {
 public:
  /**
   *  The currently active simulation
   *
   *  @return the active simulation
   */
  virtual Simulation* currentSimulation() = 0;

  /**
   *  The time offset applied to the currently active simulation
   *
   *  @return the active simulation offset
   */
  virtual const core::ClockDuration& currentIntervalOffset() = 0;

  /**
   *  Called when a significant change in the interval is detected. Subclasses
   *  must decide if the the current simulation must be switched (or updated).
   *  The result is whether the simulation was switched in this step.
   */
  virtual bool step(const core::ClockDuration& time) = 0;

  double x(const core::ClockDuration& time);

  double dx(const core::ClockDuration& time);

  bool isDone(const core::ClockDuration& time);

 private:
  core::ClockDuration _lastStep;

  void stepIfNecessary(const core::ClockDuration& time);
};

}  // namespace animation
}  // namespace rl
