// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef __RADARLOVE_ANIMATION_SPRINGSOLUTION__
#define __RADARLOVE_ANIMATION_SPRINGSOLUTION__

#include <Core/Core.h>
#include <Animation/Simulation.h>

#include <cmath>

namespace rl {
struct SpringDescription {
  /**
   *  The mass of the spring (m)
   */
  const double mass;
  /**
   *  The spring constant (k)
   */
  const double springConstant;
  /**
   *  The damping coefficient
   *  Note: Not to be confused with the damping ratio. Use the separate
   *        constructor provided for this purpose
   */
  const double damping;

  explicit SpringDescription(double mass, double springConstant, double damping)
      : mass(mass), springConstant(springConstant), damping(damping){};

  /**
   *  Create a spring given the mass, spring constant and the damping ratio. The
   *  damping ratio is especially useful in trying to determine the type of
   *  spring to create. A ratio of 1.0 creates a critically damped spring, > 1.0
   *  creates an overdamped spring and < 1.0 an underdamped one.
   *
   *  @param mass           the mass of the spring
   *  @param springConstant the spring constant
   *  @param ratio          the damping ratio
   *
   *  @return the spring description
   */
  static SpringDescription WithRatio(double mass,
                                     double springConstant,
                                     double ratio) {
    return SpringDescription(mass, springConstant,
                             ratio * 2.0 * sqrt(mass * springConstant));
  };
};

class SpringSolution : public Simulation {
 public:
  static std::unique_ptr<SpringSolution> Create(SpringDescription desc,
                                                double initialPosition,
                                                double initialVelocity);
};
}

#endif  // __RADARLOVE_ANIMATION_SPRINGSOLUTION__
