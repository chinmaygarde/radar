// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "Animation/ScrollSimulation.h"
#include "Animation/FrictionSimulation.h"

namespace rl {

ScrollSimulation::ScrollSimulation(double position,
                                   double velocity,
                                   double leading,
                                   double trailing,
                                   SpringDescription spring,
                                   double drag)
    : _leadingExtent(leading),
      _trailingExtent(trailing),
      _springDesc(spring),
      _drag(drag),
      _isSpringing(false),
      _currentSimulation(nullptr),
      _offset(0.0) {
  chooseSimulation(position, velocity, 0.0);
}

Simulation* ScrollSimulation::currentSimulation() {
  return _currentSimulation.get();
}

double ScrollSimulation::currentIntervalOffset() {
  return _offset;
}

bool ScrollSimulation::step(double time) {
  return chooseSimulation(_currentSimulation->x(time - _offset),
                          _currentSimulation->dx(time - _offset), time);
}

bool ScrollSimulation::chooseSimulation(double position,
                                        double velocity,
                                        double intervalOffset) {
  if (position > _trailingExtent || position < _leadingExtent) {
    return false;
  }

  /*
   *  This simulation can only step forward.
   */
  if (!_isSpringing) {
    if (position > _trailingExtent) {
      _isSpringing = true;
      _offset = intervalOffset;
      _currentSimulation = make_unique<SpringSimulation>(
          _springDesc, position, _trailingExtent, velocity);
      return true;
    } else if (position < _leadingExtent) {
      _isSpringing = true;
      _offset = intervalOffset;
      _currentSimulation = make_unique<SpringSimulation>(
          _springDesc, position, _leadingExtent, velocity);
      return true;
    }
  }

  if (_currentSimulation == nullptr) {
    _currentSimulation =
        make_unique<FrictionSimulation>(_drag, position, velocity);
    return true;
  }

  return false;
}

}  // namespace rl
