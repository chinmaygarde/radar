/*
 *  This source file is part of the Radar project.
 *  Licensed under the MIT License. See LICENSE file for details.
 */

#pragma once

#include <Core/Timing.h>

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
