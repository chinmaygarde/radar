// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef RADARLOVE_ANIMATION_FRICTION_SIMULATION_
#define RADARLOVE_ANIMATION_FRICTION_SIMULATION_

#include <Core/Core.h>
#include <Animation/Simulation.h>

namespace rl {
namespace animation {

class FrictionSimulation : public Simulation {
 public:
  explicit FrictionSimulation(double drag, double position, double velocity);

  double x(const core::ClockDuration& time) override;
  double dx(const core::ClockDuration& time) override;
  bool isDone(const core::ClockDuration& time) override;

 private:
  const double _drag;
  const double _dragLog;
  const double _x;
  const double _v;

  RL_DISALLOW_COPY_AND_ASSIGN(FrictionSimulation);
};

}  // namespace animation
}  // namespace rl

#endif  // RADARLOVE_ANIMATION_FRICTION_SIMULATION_