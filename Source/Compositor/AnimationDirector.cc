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

#define SpecializeSetInterpolator(type, collection)  \
  template <>                                        \
  void AnimationDirector::setInterpolator<type>(     \
      Key key, Interpolator<type> && interpolator) { \
    collection.emplace(key, interpolator);           \
  }

SpecializeSetInterpolator(double, _numberInterpolators);
SpecializeSetInterpolator(Point, _pointInterpolators);
SpecializeSetInterpolator(Size, _sizeInterpolators);
SpecializeSetInterpolator(Rect, _rectInterpolators);
SpecializeSetInterpolator(Matrix, _matrixInterpolators);
SpecializeSetInterpolator(Color, _colorInterpolators);

#undef SpecializeSetInterpolator

}  // namespace rl
