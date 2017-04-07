// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#pragma once

namespace rl {
namespace animation {

/**
 *  Acceptable tolerance for changes to velocity
 */
extern const double VelocityTolerance;
/**
 *  Acceptable tolerance for changes to distance
 */
extern const double DistanceTolerance;
/**
 *  Acceptable tolerance for changes to time intervals
 */
extern const double TimeTolerance;

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

}  // namespace animation
}  // namespace rl
