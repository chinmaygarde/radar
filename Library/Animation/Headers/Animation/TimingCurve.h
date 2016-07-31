// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef RADARLOVE_ANIMATION_TIMINGCURVE_
#define RADARLOVE_ANIMATION_TIMINGCURVE_

#include <Core/Core.h>
#include <Geometry/Geometry.h>

namespace rl {
namespace animation {

class TimingCurve {
 public:
  using Data = uint8_t;
  enum class Type : Data {
    Linear,
    EaseIn,
    EaseOut,
    EaseInEaseOut,
  };

  static TimingCurve SystemTimingCurve(Type type);

  double x(double t) const;

 private:
  double _ax;
  double _bx;
  double _cx;
  double _ay;
  double _by;
  double _cy;

  TimingCurve(const geom::Point& c1, const geom::Point& c2);
};

}  // namespace animation
}  // namespace rl

#endif  // RADARLOVE_ANIMATION_TIMINGCURVE_
