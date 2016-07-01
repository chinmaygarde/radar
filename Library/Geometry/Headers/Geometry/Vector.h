// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef RADARLOVE_GEOMETRY_VECTOR_
#define RADARLOVE_GEOMETRY_VECTOR_

#include <Geometry/Point.h>
#include <Geometry/Size.h>

#include <cmath>

namespace rl {
namespace geom {

struct Vector3 {
  double x;
  double y;
  double z;

  Vector3() : x(0.0), y(0.0), z(0.0) {}

  Vector3(const Point& p) : x(p.x), y(p.y), z(0.0) {}

  Vector3(const Size& s) : x(s.width), y(s.height), z(0.0) {}

  Vector3(double x, double y) : x(x), y(y), z(0.0) {}

  Vector3(double x, double y, double z) : x(x), y(y), z(z) {}

  /**
   *  The length (or magnitude of the vector).
   *
   *  @return the calculated length.
   */
  double length() const;

  Vector3 scale(double newLength) const;

  Vector3 normalize() const;

  double dot(const Vector3& other) const {
    return ((x * other.x) + (y * other.y) + (z * other.z));
  }

  Vector3 cross(const Vector3& other) const;

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

  /**
   *  Make a linear combination of two vectors and return the result.
   *
   *  @param a      the first vector.
   *  @param aScale the scale to use for the first vector.
   *  @param b      the second vector.
   *  @param bScale the scale to use for the second vector.
   *
   *  @return the combined vector.
   */
  static Vector3 Combine(const Vector3& a,
                         double aScale,
                         const Vector3& b,
                         double bScale);
};

struct Vector4 {
  double x;
  double y;
  double z;
  double w;

  Vector4() : x(0.0), y(0.0), z(0.0), w(1.0) {}

  Vector4(double x, double y, double z, double w) : x(x), y(y), z(z), w(w) {}

  Vector4(const Vector3& v) : x(v.x), y(v.y), z(v.z), w(1.0) {}

  Vector4(const Point& p) : x(p.x), y(p.y), z(0.0), w(1.0) {}

  Vector4 normalize() const {
    const double inverse = 1.0 / sqrt(x * x + y * y + z * z + w * w);
    return Vector4(x * inverse, y * inverse, z * inverse, w * inverse);
  }

  bool operator==(const Vector4& v) const {
    return (x == v.x) && (y == v.y) && (z == v.z) && (w == v.w);
  }

  bool operator!=(const Vector4& v) const {
    return (x != v.x) || (y != v.y) || (z != v.z) || (w != v.w);
  }

  Vector4 operator+(const Vector4& v) const {
    return Vector4(x + v.x, y + v.y, z + v.z, w + v.w);
  }

  Vector4 operator-(const Vector4& v) const {
    return Vector4(x - v.x, y - v.y, z - v.z, w - v.w);
  }
};

}  // namespace geom
}  // namespace rl

#endif  // RADARLOVE_GEOMETRY_VECTOR_
