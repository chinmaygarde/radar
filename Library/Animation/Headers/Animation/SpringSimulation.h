// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef RADARLOVE_ANIMATION_SPRINGSIMULATION_
#define RADARLOVE_ANIMATION_SPRINGSIMULATION_

#include <Animation/Simulation.h>
#include <Animation/SpringSolution.h>
#include <Core/Core.h>
#include <cmath>

namespace rl {
namespace animation {

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

  double x(const core::ClockDuration& time) override;
  double dx(const core::ClockDuration& time) override;
  bool isDone(const core::ClockDuration& time) override;

 private:
  const double _endPosition;
  const std::unique_ptr<SpringSolution> _solution;

  RL_DISALLOW_COPY_AND_ASSIGN(SpringSimulation);
};

}  // namespace animation
}  // namespace rl

#endif  // RADARLOVE_ANIMATION_SPRINGSIMULATION_
