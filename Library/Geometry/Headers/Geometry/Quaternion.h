// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef RADAR_GEOMETRY_QUATERNION_H_
#define RADAR_GEOMETRY_QUATERNION_H_

#include <Geometry/Vector.h>

namespace rl {
namespace geom {

struct Quaternion {
  double x;
  double y;
  double z;
  double w;

  Quaternion() : x(0.0), y(0.0), z(0.0), w(1.0) {}

  Quaternion(double px, double py, double pz, double pw)
      : x(px), y(py), z(pz), w(pw) {}

  Quaternion(const Vector3& axis, double angle) {
    auto sine = sin(angle * 0.5);
    x = sine * axis.x;
    y = sine * axis.y;
    z = sine * axis.z;
    w = cos(angle * 0.5);
  }

  bool operator==(const Quaternion& o) const {
    return x == o.x && y == o.y && z == o.z && w == o.w;
  }

  bool operator!=(const Quaternion& o) const {
    return x != o.x || y != o.y || z != o.z || w != o.w;
  }
};

}  // namespace geom
}  // namespace rl

#endif  // RADAR_GEOMETRY_QUATERNION_H_
