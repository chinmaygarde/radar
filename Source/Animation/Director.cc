// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

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
Director::Interpolators<coordinator::Color>& Director::collection() {
  return _colorInterpolators;
}

template <>
Director::Interpolators<geom::Matrix>& Director::collection() {
  return _matrixInterpolators;
}

}  // namespace animation
}  // namespace rl
