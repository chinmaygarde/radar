/*
 *  This source file is part of the Radar project.
 *  Licensed under the MIT License. See LICENSE file for details.
 */

#include <Animation/FrictionSimulation.h>
#include <Animation/ScrollSimulation.h>
#include <Core/Utilities.h>

namespace rl {
namespace animation {

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
  chooseSimulation(position, velocity, core::ClockDuration::zero());
}

Simulation* ScrollSimulation::currentSimulation() {
  return _currentSimulation.get();
}

const core::ClockDuration& ScrollSimulation::currentIntervalOffset() {
  return _offset;
}

bool ScrollSimulation::step(const core::ClockDuration& time) {
  return chooseSimulation(_currentSimulation->x(time - _offset),
                          _currentSimulation->dx(time - _offset), time);
}

bool ScrollSimulation::chooseSimulation(
    double position,
    double velocity,
    const core::ClockDuration& intervalOffset) {
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
      _currentSimulation = std::make_unique<SpringSimulation>(
          _springDesc, position, _trailingExtent, velocity);
      return true;
    } else if (position < _leadingExtent) {
      _isSpringing = true;
      _offset = intervalOffset;
      _currentSimulation = std::make_unique<SpringSimulation>(
          _springDesc, position, _leadingExtent, velocity);
      return true;
    }
  }

  if (_currentSimulation == nullptr) {
    _currentSimulation =
        std::make_unique<FrictionSimulation>(_drag, position, velocity);
    return true;
  }

  return false;
}

}  // namespace animation
}  // namespace rl
