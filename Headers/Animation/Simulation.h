// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef __RADARLOVE_ANIMATION_SIMULATION__
#define __RADARLOVE_ANIMATION_SIMULATION__

#include "Core/Base.h"

namespace rl {
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
  virtual double x(double time) = 0;

  /**
   *  The current velocity of the object in the simulation
   *
   *  @param time the time in the simulation
   *
   *  @return the velocity
   */
  virtual double dx(double time) = 0;

  /**
   *  Returns if the simulation is done at a given time
   *
   *  @param time the time in the simulation
   *
   *  @return if the simulation is done
   */
  virtual bool isDone(double time) = 0;
};
}

#endif /* defined(__RADARLOVE_ANIMATION_SIMULATION__) */
