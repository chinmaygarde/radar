// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef __RADARLOVE_COMPOSITOR_TIMINGCURVE__
#define __RADARLOVE_COMPOSITOR_TIMINGCURVE__

#include "Core/Base.h"
#include "Geometry/Point.h"

namespace rl {
class TimingCurve {
 public:
  enum Type {
    Linear,
    EaseIn,
    EaseOut,
    EaseInEaseOut,
  };

  TimingCurve(const Point& c1, const Point& c2);

  static const TimingCurve& SystemTimingCurve(Type type);

  double x(double t) const;

 private:
  double _ax;
  double _bx;
  double _cx;

  double _ay;
  double _by;
  double _cy;
  DISALLOW_COPY_AND_ASSIGN(TimingCurve);
};
}

#endif /* defined(__RADARLOVE_COMPOSITOR_TIMINGCURVE__) */