// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <Compositor/AnimationDirector.h>

namespace rl {

AnimationDirector::AnimationDirector()
    : _numberInterpolators(),
      _pointInterpolators(),
      _sizeInterpolators(),
      _rectInterpolators(),
      _matrixInterpolators(),
      _colorInterpolators() {
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
