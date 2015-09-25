// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <Compositor/AnimationDirector.h>

namespace rl {

AnimationDirector::AnimationDirector() {
}

size_t AnimationDirector::flushInterpolations(Stopwatch& stopwatch) {
  AutoStopwatchLap lap(stopwatch);

  const ClockPoint time = Clock::now();
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
AnimationDirector::Interpolators<Size>& AnimationDirector::collection() {
  return _sizeInterpolators;
}

template <>
AnimationDirector::Interpolators<Point>& AnimationDirector::collection() {
  return _pointInterpolators;
}

template <>
AnimationDirector::Interpolators<Rect>& AnimationDirector::collection() {
  return _rectInterpolators;
}

template <>
AnimationDirector::Interpolators<Color>& AnimationDirector::collection() {
  return _colorInterpolators;
}

template <>
AnimationDirector::Interpolators<Matrix>& AnimationDirector::collection() {
  return _matrixInterpolators;
}

}  // namespace rl
