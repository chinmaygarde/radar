// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef RADARLOVE_GEOMETRY_POINT_
#define RADARLOVE_GEOMETRY_POINT_

#include <Geometry/Size.h>

#include <string>

namespace rl {
namespace geom {

struct Point {
  double x;
  double y;

  Point() : x(0.0), y(0.0) {}
  Point(double x, double y) : x(x), y(y) {}

  /*
   *  Operator overloads
   */
  bool operator==(const Point& p) const { return p.x == x && p.y == y; }
  bool operator!=(const Point& p) const { return p.x != x || p.y != y; }

  Point operator-() const { return Point(-x, -y); }

  Point operator+(const Point& p) const { return Point(x + p.x, y + p.y); }
  Point operator+(const Size& s) const {
    return Point(x + s.width, y + s.height);
  }

  Point operator-(const Point& p) const { return Point(x - p.x, y - p.y); }
  Point operator-(const Size& s) const {
    return Point(x - s.width, y - s.height);
  }

  Point operator*(double scale) const { return Point(x * scale, y * scale); }
  Point operator*(const Point& p) const { return Point(x * p.x, y * p.y); }
  Point operator*(const Size& s) const {
    return Point(x * s.width, y * s.height);
  }

  Point operator/(const Point& p) const { return Point(x / p.x, y / p.y); }
  Point operator/(const Size& s) const {
    return Point(x / s.width, y / s.height);
  }

  std::string toString() const;

  void fromString(const std::string& str);
};

static const Point PointZero(0.0, 0.0);

}  // namespace geom
}  // namespace rl

#endif  // RADARLOVE_GEOMETRY_POINT_
