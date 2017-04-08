/*
 *  This source file is part of the Radar project.
 *  Licensed under the MIT License. See LICENSE file for details.
 */

#include <Geometry/Quaternion.h>

namespace rl {
namespace geom {

Quaternion Quaternion::slerp(const Quaternion& to, double time) const {
  double cosine = dot(to);
  if (fabs(cosine) < 1.0 - 1e-3 /* epsilon */) {
    /*
     *  Spherical Interpolation.
     */
    auto sine = sqrt(1.0 - cosine * cosine);
    auto angle = atan2(sine, cosine);
    auto sineInverse = 1.0 / sine;
    auto c0 = sin((1.0 - time) * angle) * sineInverse;
    auto c1 = sin(time * angle) * sineInverse;
    return *this * c0 + to * c1;
  } else {
    /*
     *  Linear Interpolation.
     */
    return (*this * (1.0 - time) + to * time).normalize();
  }
}

}  // namespace geom
}  // namespace rl
