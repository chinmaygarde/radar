// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef __RADARLOVE_ANIMATION_SIMULATIONGROUP__
#define __RADARLOVE_ANIMATION_SIMULATIONGROUP__

#include <Core/Core.h>
#include "Animation/Simulation.h"
#include "Animation/Utilities.h"

namespace rl {
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
  virtual double currentIntervalOffset() = 0;

  /**
   *  Called when a significant change in the interval is detected. Subclasses
   *  must decide if the the current simulation must be switched (or updated).
   *  The result is whether the simulation was switched in this step.
   */
  virtual bool step(double time) = 0;

  virtual double x(double time) override;

  virtual double dx(double time) override;

  virtual bool isDone(double time) override;

 private:
  double _lastStep = -1.0;

  void stepIfNecessary(double time);
};
}

#endif /* defined(__RADARLOVE_ANIMATION_SIMULATIONGROUP__) */
