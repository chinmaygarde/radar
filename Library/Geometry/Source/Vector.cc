/*
 *  This source file is part of the Radar project.
 *  Licensed under the MIT License. See LICENSE file for details.
 */

#include <Geometry/Vector.h>

namespace rl {
namespace geom {

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
