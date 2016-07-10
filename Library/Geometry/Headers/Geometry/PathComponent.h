// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef RADAR_GEOMETRY_PATHCOMPONENT_H_
#define RADAR_GEOMETRY_PATHCOMPONENT_H_

#include <Core/Macros.h>
#include <Geometry/Point.h>

namespace rl {
namespace geom {

struct LinearPathComponent {
  Point p1;
  Point p2;

  LinearPathComponent(Point ap1, Point ap2) : p1(ap1), p2(ap2) {}

  Point solve(double time) const;
};

struct QuadraticPathComponent {
  Point p1;
  Point cp;
  Point p2;

  QuadraticPathComponent(Point ap1, Point acp, Point ap2)
      : p1(ap1), cp(acp), p2(ap2) {}

  Point solve(double time) const;
};

struct CubicPathComponent {
  Point p1;
  Point cp1;
  Point cp2;
  Point p2;

  CubicPathComponent(Point ap1, Point acp1, Point acp2, Point ap2)
      : p1(ap1), cp1(acp1), cp2(acp2), p2(ap2) {}

  Point solve(double time) const;
};

}  // namespace geom
}  // namespace rl

#endif  // RADAR_GEOMETRY_PATHCOMPONENT_H_
