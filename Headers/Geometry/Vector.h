// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef __RADARLOVE_GEOMETRY_VECTOR__
#define __RADARLOVE_GEOMETRY_VECTOR__

#include "Geometry/Point.h"

namespace rl {
struct Vector4 {
  double a;
  double b;
  double c;
  double d;

  Vector4(double a, double b, double c, double d) : a(a), b(b), c(c), d(d) {}

  bool operator==(const Vector4& v) const {
    return a == v.a && b == v.b && c == v.c && d == v.d;
  }

  bool operator!=(const Vector4& v) const {
    return a != v.a || b != v.b || c != v.c || d != v.d;
  }

  Vector4 operator+(const Vector4& v) const {
    return Vector4(a + v.a, b + v.b, c + v.c, d + v.d);
  }

  Vector4 operator-(const Vector4& v) const {
    return Vector4(a - v.a, b - v.b, c - v.c, d - v.d);
  }
};

struct Vector3 {
  double x;
  double y;
  double z;

  Vector3(const Point& p) : x(p.x), y(p.y) {}

  Vector3(double x, double y) : x(x), y(y), z(0.0) {}

  Vector3(double x, double y, double z) : x(x), y(y), z(z) {}

  bool operator==(const Vector3& v) const {
    return v.x == x && v.y == y && v.z == z;
  }

  bool operator!=(const Vector3& v) const {
    return v.x != x || v.y != y || v.z != z;
  }

  Vector3 operator-() const { return Vector3(-x, -y, -z); }

  Vector3 operator+(const Vector3& v) const {
    return Vector3(x + v.x, y + v.y, z + v.z);
  }

  Vector3 operator-(const Vector3& v) const {
    return Vector3(x - v.x, y - v.y, z - v.z);
  }
};
}

#endif /* defined(__RADARLOVE_GEOMETRY_VECTOR__) */
