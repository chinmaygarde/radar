/*
 *  This source file is part of the Radar project.
 *  Licensed under the MIT License. See LICENSE file for details.
 */

#pragma once

#include <Animation/Simulation.h>
#include <Core/Macros.h>

namespace rl {
namespace animation {

class FrictionSimulation : public Simulation {
 public:
  FrictionSimulation(double drag, double position, double velocity);

  double x(const core::ClockDuration& time) const override;

  double dx(const core::ClockDuration& time) const override;

  bool isDone(const core::ClockDuration& time) const override;

 private:
  const double _drag;
  const double _dragLog;
  const double _x;
  const double _v;

  RL_DISALLOW_COPY_AND_ASSIGN(FrictionSimulation);
};

}  // namespace animation
}  // namespace rl
