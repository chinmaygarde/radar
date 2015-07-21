// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef __RADARLOVE_GEOMETRY_VECTOR__
#define __RADARLOVE_GEOMETRY_VECTOR__

namespace rl {
struct Vector {
  double a;
  double b;
  double c;
  double d;

  Vector(double a, double b, double c, double d) : a(a), b(b), c(c), d(d) {}

  bool operator==(const Vector& v) const {
    return a == v.a && b == v.b && c == v.c && d == v.d;
  }

  Vector operator+(const Vector& v) const {
    return Vector(a + v.a, b + v.b, c + v.c, d + v.d);
  }

  Vector operator-(const Vector& v) const {
    return Vector(a - v.a, b - v.b, c - v.c, d - v.d);
  }
};
}

#endif /* defined(__RADARLOVE_GEOMETRY_VECTOR__) */
