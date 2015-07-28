// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef __RADARLOVE_ANIMATION_SCROLLSIMULATION__
#define __RADARLOVE_ANIMATION_SCROLLSIMULATION__

#include "Core/Base.h"
#include "Animation/SimulationGroup.h"
#include "Animation/SpringSimulation.h"

namespace rl {
/**
 *  Simulates kinetic scrolling behavior between a leading and trailing
 *  boundary. Friction is applied within the extends and a spring action applied
 *  at the boundaries. This simulation can only step forward.
 */
class ScrollSimulation : public SimulationGroup {
 public:
  ScrollSimulation(double position,
                   double velocity,
                   double leading,
                   double trailing,
                   SpringDescription spring,
                   double drag);

 protected:
  virtual Simulation* currentSimulation() override;

  virtual double currentIntervalOffset() override;

  virtual bool step(double time) override;

 private:
  bool chooseSimulation(double position,
                        double velocity,
                        double intervalOffset);

  const double _leadingExtent;
  const double _trailingExtent;
  const SpringDescription _springDesc;
  const double _drag;
  bool _isSpringing;
  std::unique_ptr<Simulation> _currentSimulation;
  double _offset;

  DISALLOW_COPY_AND_ASSIGN(ScrollSimulation);
};
}

#endif /* defined(__RADARLOVE_ANIMATION_SCROLLSIMULATION__) */
