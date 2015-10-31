// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <Coordinator/TimingCurve.h>

#include <cmath>

namespace rl {
namespace coordinator {

static inline double TimingCurve_SampleCurve(double a,
                                             double b,
                                             double c,
                                             double t) {
  /*
   *  `a t^3 + b t^2 + c t' expanded using Horner's rule.
   */
  return ((a * t + b) * t + c) * t;
}

static inline double TimingCurve_SampleCurveDerivative(double a,
                                                       double b,
                                                       double c,
                                                       double t) {
  return (3.0 * a * t + 2.0 * b) * t + c;
}

static inline double TimingCurve_SolveCurveX(double ax,
                                             double bx,
                                             double cx,
                                             double x,
                                             double epsilon) {
  double t0;
  double t1;
  double t2;
  double x2;
  double d2;
  int i;

  /*
   *  Try Newton's method
   */
  for (t2 = x, i = 0; i < 8; i++) {
    x2 = TimingCurve_SampleCurve(ax, bx, cx, t2) - x;

    if (fabs(x2) < epsilon) {
      return t2;
    }

    d2 = TimingCurve_SampleCurveDerivative(ax, bx, cx, t2);

    if (fabs(d2) < epsilon) {
      break;
    }

    t2 = t2 - x2 / d2;
  }

  /*
   *  Fall back to bisection
   */
  t0 = 0.0;
  t1 = 1.0;
  t2 = x;

  if (t2 < t0) {
    return t0;
  }

  if (t2 > t1) {
    return t1;
  }

  while (t0 < t1) {
    x2 = TimingCurve_SampleCurve(ax, bx, cx, t2);

    if (fabs(x2 - x) < epsilon)
      return t2;

    if (x > x2) {
      t0 = t2;
    } else {
      t1 = t2;
    }

    t2 = (t1 - t0) * 0.5 + t0;
  }

  /*
   *  Failure
   */
  return t2;
}

static inline double TimingCurve_SolveX(double ax,
                                        double bx,
                                        double cx,
                                        double ay,
                                        double by,
                                        double cy,
                                        double x,
                                        double epsilon) {
  const double xSolution = TimingCurve_SolveCurveX(ax, bx, cx, x, epsilon);
  return TimingCurve_SampleCurve(ay, by, cy, xSolution);
}

TimingCurve TimingCurve::SystemTimingCurve(Type type) {
  switch (type) {
    case Linear:
      return TimingCurve({0.0, 0.0}, {1.0, 1.0});
    case EaseIn:
      return TimingCurve({0.42, 0.0}, {1.0, 1.0});
    case EaseOut:
      return TimingCurve({0.0, 0.0}, {0.58, 1.0});
    case EaseInEaseOut:
      return TimingCurve({0.42, 0.0}, {0.58, 1.0});
  }

  return TimingCurve({0.0, 0.0}, {1.0, 1.0});
}

TimingCurve::TimingCurve(const geom::Point& c1, const geom::Point& c2) {
  _cx = 3.0 * c1.x;
  _bx = 3.0 * (c2.x - c1.x) - _cx;
  _ax = 1.0 - _cx - _bx;

  _cy = 3.0 * c1.y;
  _by = 3.0 * (c2.y - c1.y) - _cy;
  _ay = 1.0 - _cy - _by;
}

double TimingCurve::x(double t) const {
  return TimingCurve_SolveX(_ax, _bx, _cx, _ay, _by, _cy, t, 1e-3);
}

}  // namespace coordinator
}  // namespace rl
