// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef RADARLOVE_ANIMATION_SIMULATIONGROUP_
#define RADARLOVE_ANIMATION_SIMULATIONGROUP_

#include <Core/Core.h>
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
class SimulationGroup : public Simulation {
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

  double x(const core::ClockDuration& time) override;

  double dx(const core::ClockDuration& time) override;

  bool isDone(const core::ClockDuration& time) override;

 private:
  core::ClockDuration _lastStep;

  void stepIfNecessary(const core::ClockDuration& time);
};

}  // namespace animation
}  // namespace rl

#endif  // RADARLOVE_ANIMATION_SIMULATIONGROUP_
