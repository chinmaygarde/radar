/*
 *  This source file is part of the Radar project.
 *  Licensed under the MIT License. See LICENSE file for details.
 */

#include <Animation/Director.h>

namespace rl {
namespace animation {

Director::Director() {}

size_t Director::stepInterpolations(instrumentation::Stopwatch& stopwatch) {
  instrumentation::AutoStopwatchLap lap(stopwatch);

  const core::ClockPoint time = core::Clock::now();
  size_t count = 0;

  for (const auto& i : _numberInterpolators) {
    i.second->step(time);
  }
  count += _numberInterpolators.size();

  for (const auto& i : _pointInterpolators) {
    i.second->step(time);
  }
  count += _pointInterpolators.size();

  for (const auto& i : _sizeInterpolators) {
    i.second->step(time);
  }
  count += _sizeInterpolators.size();

  for (const auto& i : _rectInterpolators) {
    i.second->step(time);
  }
  count += _rectInterpolators.size();

  for (const auto& i : _matrixInterpolators) {
    i.second->step(time);
  }
  count += _matrixInterpolators.size();

  for (const auto& i : _colorInterpolators) {
    i.second->step(time);
  }
  count += _colorInterpolators.size();

  return count;
}

template <>
Director::Interpolators<double>& Director::collection() {
  return _numberInterpolators;
}

template <>
Director::Interpolators<geom::Size>& Director::collection() {
  return _sizeInterpolators;
}

template <>
Director::Interpolators<geom::Point>& Director::collection() {
  return _pointInterpolators;
}

template <>
Director::Interpolators<geom::Rect>& Director::collection() {
  return _rectInterpolators;
}

template <>
Director::Interpolators<entity::ColorHSB>& Director::collection() {
  return _colorInterpolators;
}

template <>
Director::Interpolators<geom::Matrix::Decomposition>& Director::collection() {
  return _matrixInterpolators;
}

}  // namespace animation
}  // namespace rl
