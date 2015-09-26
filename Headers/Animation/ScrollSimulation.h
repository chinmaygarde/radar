// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef RADARLOVE_ANIMATION_SCROLLSIMULATION_
#define RADARLOVE_ANIMATION_SCROLLSIMULATION_

#include <Core/Core.h>
#include <Animation/SimulationGroup.h>
#include <Animation/SpringSimulation.h>

namespace rl {
/**
 *  Simulates kinetic scrolling behavior between a leading and trailing
 *  boundary. Friction is applied within the extends and a spring action applied
 *  at the boundaries. This simulation can only step forward.
 */
class ScrollSimulation : public SimulationGroup {
 public:
  explicit ScrollSimulation(double position,
                            double velocity,
                            double leading,
                            double trailing,
                            SpringDescription spring,
                            double drag);

 protected:
  Simulation* currentSimulation() override;

  const ClockDuration& currentIntervalOffset() override;

  bool step(const ClockDuration& time) override;

 private:
  bool chooseSimulation(double position,
                        double velocity,
                        const ClockDuration& intervalOffset);

  const double _leadingExtent;
  const double _trailingExtent;
  const SpringDescription _springDesc;
  const double _drag;
  bool _isSpringing;
  std::unique_ptr<Simulation> _currentSimulation;
  ClockDuration _offset;

  RL_DISALLOW_COPY_AND_ASSIGN(ScrollSimulation);
};
}  // namespace rl

#endif  // RADARLOVE_ANIMATION_SCROLLSIMULATION_
