/*
 *  This source file is part of the Radar project.
 *  Licensed under the MIT License. See LICENSE file for details.
 */

#pragma once

#include <Animation/Simulation.h>
#include <Core/Macros.h>

namespace rl {
namespace animation {

class GravitySimulation : public Simulation {
 public:
  explicit GravitySimulation(double acceleration,
                             double distance,
                             double endDistance,
                             double velocity);

  double x(const core::ClockDuration& time) const override;

  double dx(const core::ClockDuration& time) const override;

  bool isDone(const core::ClockDuration& time) const override;

 private:
  const double _a;
  const double _x;
  const double _v;
  const double _end;

  RL_DISALLOW_COPY_AND_ASSIGN(GravitySimulation);
};

}  // namespace animation
}  // namespace rl
