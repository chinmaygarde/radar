// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <Coordinator/AnimationDirector.h>

namespace rl {
namespace coordinator {

AnimationDirector::AnimationDirector() {
}

size_t AnimationDirector::stepInterpolations(
    instrumentation::Stopwatch& stopwatch) {
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
AnimationDirector::Interpolators<double>& AnimationDirector::collection() {
  return _numberInterpolators;
}

template <>
AnimationDirector::Interpolators<geom::Size>& AnimationDirector::collection() {
  return _sizeInterpolators;
}

template <>
AnimationDirector::Interpolators<geom::Point>& AnimationDirector::collection() {
  return _pointInterpolators;
}

template <>
AnimationDirector::Interpolators<geom::Rect>& AnimationDirector::collection() {
  return _rectInterpolators;
}

template <>
AnimationDirector::Interpolators<Color>& AnimationDirector::collection() {
  return _colorInterpolators;
}

template <>
AnimationDirector::Interpolators<geom::Matrix>&
AnimationDirector::collection() {
  return _matrixInterpolators;
}

}  // namespace coordinator
}  // namespace rl
