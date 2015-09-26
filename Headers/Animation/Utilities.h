// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef RADARLOVE_ANIMATION_UTILITIES_
#define RADARLOVE_ANIMATION_UTILITIES_

namespace rl {
namespace Animation {
/**
 *  Acceptable tolerance for changes to velocity
 */
extern const float VelocityTolerance;
/**
 *  Acceptable tolerance for changes to distance
 */
extern const float DistanceTolerance;
/**
 *  Acceptable tolerance for changes to time intervals
 */
extern const float TimeTolerance;

/**
 *  Check equality within the specified tolerance
 *
 *  @param a       first value
 *  @param b       second value
 *  @param epsilon tolerance
 *
 *  @return if equal within the tolerance
 */
bool NearEqual(double a, double b, double epsilon);

/**
 *  Check equality to zero within the specified tolerance
 *
 *  @param a       the value
 *  @param epsilon tolerance
 *
 *  @return if equal to zero within the tolerance
 */
bool NearZero(double a, double epsilon);
}  // namespace Animation
}  // namespace rl

#endif  // RADARLOVE_ANIMATION_UTILITIES_
