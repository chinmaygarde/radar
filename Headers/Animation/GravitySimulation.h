// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef __RADARLOVE_ANIMATION_GRAVITYSIMULATION__
#define __RADARLOVE_ANIMATION_GRAVITYSIMULATION__

#include <Core/Core.h>
#include "Simulation.h"

namespace rl {
class GravitySimulation : public Simulation {
 public:
  GravitySimulation(double acceleration,
                    double distance,
                    double endDistance,
                    double velocity);

  virtual double x(double time) override;
  virtual double dx(double time) override;
  virtual bool isDone(double time) override;

 private:
  const double _a;
  const double _x;
  const double _v;
  const double _end;

  DISALLOW_COPY_AND_ASSIGN(GravitySimulation);
};
}

#endif /* defined(__RADARLOVE_ANIMATION_GRAVITYSIMULATION__) */
