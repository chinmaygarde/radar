// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef __RADARLOVE_ANIMATION_SPRINGSIMULATION__
#define __RADARLOVE_ANIMATION_SPRINGSIMULATION__

#include "Core/Base.h"
#include "Animation/Simulation.h"
#include "Animation/SpringSolution.h"

#include <cmath>

namespace rl {
/**
 * A spring simulation. Depending on the spring description, a critically,
 * under or overdamped spring will be created.
 */
class SpringSimulation : public Simulation {
 public:
  /**
   *  A spring description with the provided spring description, start distance,
   *  end distance and velocity.
   */
  SpringSimulation(SpringDescription desc,
                   double start,
                   double end,
                   double velocity);

  virtual double x(double time) override;
  virtual double dx(double time) override;
  virtual bool isDone(double time) override;

 private:
  const double _endPosition;
  const std::unique_ptr<SpringSolution> _solution;

  DISALLOW_COPY_AND_ASSIGN(SpringSimulation);
};
}

#endif /* defined(__RADARLOVE_ANIMATION_SPRINGSIMULATION__) */
