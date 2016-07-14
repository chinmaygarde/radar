// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <Geometry/Vector.h>

namespace rl {
namespace geom {

double Vector3::length() const {
  return sqrt(x * x + y * y + z * z);
}

Vector3 Vector3::Combine(const Vector3& a,
                         double aScale,
                         const Vector3& b,
                         double bScale) {
  return {
      aScale * a.x + bScale * b.x,  //
      aScale * a.y + bScale * b.y,  //
      aScale * a.z + bScale * b.z,  //
  };
}

Vector3 Vector3::cross(const Vector3& other) const {
  return {
      (y * other.z) - (z * other.y),  //
      (z * other.x) - (x * other.z),  //
      (x * other.y) - (y * other.x)   //
  };
}

Vector3 Vector3::normalize() const {
  const auto len = length();
  return {x / len, y / len, z / len};
}

}  // namespace geom
}  // namespace rl
