// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef RADARLOVE_COMPOSITOR_TIMINGCURVE_
#define RADARLOVE_COMPOSITOR_TIMINGCURVE_

#include <Core/Core.h>
#include <Geometry/Geometry.h>

namespace rl {
class TimingCurve {
 public:
  enum Type {
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

  explicit TimingCurve(const Point& c1, const Point& c2);
};
}  // namespace rl

#endif  // RADARLOVE_COMPOSITOR_TIMINGCURVE_
