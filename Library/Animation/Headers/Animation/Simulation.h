// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef RADARLOVE_ANIMATION_SIMULATION_
#define RADARLOVE_ANIMATION_SIMULATION_

#include <Core/Core.h>

namespace rl {
namespace animation {

/**
 *  The base class for all simulations. The user is meant to instantiate an
 *  instance of a simulation and query the same for the position and velocity
 *  of the body at a given interval.
 */
class Simulation {
 public:
  /**
   *  The current position of the object in the simulation
   *
   *  @param time the time in the simulation
   *
   *  @return the position
   */
  virtual double x(const core::ClockDuration& time) = 0;

  /**
   *  The current velocity of the object in the simulation
   *
   *  @param time the time in the simulation
   *
   *  @return the velocity
   */
  virtual double dx(const core::ClockDuration& time) = 0;

  /**
   *  Returns if the simulation is done at a given time
   *
   *  @param time the time in the simulation
   *
   *  @return if the simulation is done
   */
  virtual bool isDone(const core::ClockDuration& time) = 0;
};

}  // namespace animation
}  // namespace rl

#endif  // RADARLOVE_ANIMATION_SIMULATION_