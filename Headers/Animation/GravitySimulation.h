// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef __RADARLOVE_ANIMATION_GRAVITYSIMULATION__
#define __RADARLOVE_ANIMATION_GRAVITYSIMULATION__

#include <Core/Core.h>
#include <Animation/Simulation.h>

namespace rl {
class GravitySimulation : public Simulation {
 public:
  explicit GravitySimulation(double acceleration,
                             double distance,
                             double endDistance,
                             double velocity);

  virtual double x(const ClockDuration& time) override;
  virtual double dx(const ClockDuration& time) override;
  virtual bool isDone(const ClockDuration& time) override;

 private:
  const double _a;
  const double _x;
  const double _v;
  const double _end;

  RL_DISALLOW_COPY_AND_ASSIGN(GravitySimulation);
};
}

#endif /* defined(__RADARLOVE_ANIMATION_GRAVITYSIMULATION__) */
