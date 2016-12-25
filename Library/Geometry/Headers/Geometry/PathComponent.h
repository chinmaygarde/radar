// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef RADAR_GEOMETRY_PATHCOMPONENT_H_
#define RADAR_GEOMETRY_PATHCOMPONENT_H_

#include <Core/Macros.h>
#include <Geometry/Point.h>
#include <vector>

namespace rl {
namespace geom {

struct LinearPathComponent {
  Point p1;
  Point p2;

  LinearPathComponent() {}

  LinearPathComponent(Point ap1, Point ap2) : p1(ap1), p2(ap2) {}

  Point solve(double time) const;

  bool operator==(const LinearPathComponent& other) const {
    return p1 == other.p1 && p2 == other.p2;
  };
};

struct TessellationApproximation {
  double scale;
  double angleTolerance;
  double cuspLimit;
  double distanceToleranceSquare;

  TessellationApproximation(/* default */)
      : TessellationApproximation(1.0 /* scale */,
                                  0.0 /* angle tolerance */,
                                  0.0 /* cusp limit */) {}

  TessellationApproximation(double pScale,
                            double pAngleTolerance,
                            double pCuspLimit)
      : scale(pScale),
        angleTolerance(pAngleTolerance),
        cuspLimit(pCuspLimit),
        distanceToleranceSquare(0.5 * pScale * 0.5 * pScale) {}
};

struct QuadraticPathComponent {
  Point p1;
  Point cp;
  Point p2;

  QuadraticPathComponent() {}

  QuadraticPathComponent(Point ap1, Point acp, Point ap2)
      : p1(ap1), cp(acp), p2(ap2) {}

  Point solve(double time) const;

  Point solveDerivative(double time) const;

  std::vector<Point> tessellate(
      const TessellationApproximation& approximation) const;

  bool operator==(const QuadraticPathComponent& other) const {
    return p1 == other.p1 && cp == other.cp && p2 == other.p2;
  }
};

struct CubicPathComponent {
  Point p1;
  Point cp1;
  Point cp2;
  Point p2;

  CubicPathComponent() {}

  CubicPathComponent(Point ap1, Point acp1, Point acp2, Point ap2)
      : p1(ap1), cp1(acp1), cp2(acp2), p2(ap2) {}

  Point solve(double time) const;

  Point solveDerivative(double time) const;

  std::vector<Point> tessellate(
      const TessellationApproximation& approximation) const;

  bool operator==(const CubicPathComponent& other) const {
    return p1 == other.p1 && cp1 == other.cp1 && cp2 == other.cp2 &&
           p2 == other.p2;
  }
};

}  // namespace geom
}  // namespace rl

#endif  // RADAR_GEOMETRY_PATHCOMPONENT_H_
