/*
 *  This source file is part of the Radar project.
 *  Licensed under the MIT License. See LICENSE file for details.
 */

#pragma once

#include <Animation/Simulation.h>
#include <Animation/SpringSolution.h>
#include <Core/Macros.h>
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

  double x(const core::ClockDuration& time) const override;

  double dx(const core::ClockDuration& time) const override;

  bool isDone(const core::ClockDuration& time) const override;

 private:
  const double _endPosition;
  const std::unique_ptr<SpringSolution> _solution;

  RL_DISALLOW_COPY_AND_ASSIGN(SpringSimulation);
};

}  // namespace animation
}  // namespace rl
