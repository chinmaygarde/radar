// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <Animation/Utilities.h>

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
