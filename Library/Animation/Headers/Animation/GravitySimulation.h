// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef RADARLOVE_ANIMATION_GRAVITYSIMULATION_
#define RADARLOVE_ANIMATION_GRAVITYSIMULATION_

#include <Animation/Simulation.h>
#include <Core/Core.h>

namespace rl {
namespace animation {

class GravitySimulation : public Simulation {
 public:
  explicit GravitySimulation(double acceleration,
                             double distance,
                             double endDistance,
                             double velocity);

  double x(const core::ClockDuration& time) override;
  double dx(const core::ClockDuration& time) override;
  bool isDone(const core::ClockDuration& time) override;

 private:
  const double _a;
  const double _x;
  const double _v;
  const double _end;

  RL_DISALLOW_COPY_AND_ASSIGN(GravitySimulation);
};

}  // namespace animation
}  // namespace rl

#endif  // RADARLOVE_ANIMATION_GRAVITYSIMULATION_
