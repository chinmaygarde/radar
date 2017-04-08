/*
 *  This source file is part of the Radar project.
 *  Licensed under the MIT License. See LICENSE file for details.
 */

#include <Animation/AnimationUtilities.h>

namespace rl {
namespace animation {

const double VelocityTolerance = 1e-3;
const double DistanceTolerance = 1e-3;
const double TimeTolerance = 1e-3;

bool NearEqual(double a, double b, double epsilon) {
  return (a > (b - epsilon)) && (a < (b + epsilon));
}

bool NearZero(double a, double epsilon) {
  return NearEqual(a, 0.0, epsilon);
}

}  // namespace animation
}  // namespace rl
