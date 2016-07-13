// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <Geometry/PathComponent.h>

namespace rl {
namespace geom {

static inline double LinearSolve(double t, double p0, double p1) {
  return p0 + t * (p1 - p0);
}

static inline double QuadraticSolve(double t, double p0, double p1, double p2) {
  return (1 - t) * (1 - t) * p0 +  //
         2 * (1 - t) * t * p1 +    //
         t * t * p2;
}

static inline double QuadraticSolveDerivative(double t,
                                              double p0,
                                              double p1,
                                              double p2) {
  return 2 * (1 - t) * (p1 - p0) +  //
         2 * t * (p2 - p1);
}

static inline double CubicSolve(double t,
                                double p0,
                                double p1,
                                double p2,
                                double p3) {
  return (1 - t) * (1 - t) * (1 - t) * p0 +  //
         3 * (1 - t) * (1 - t) * t * p1 +    //
         3 * (1 - t) * t * t * p2 +          //
         t * t * t * p3;
}

static inline double CubicSolveDerivative(double t,
                                          double p0,
                                          double p1,
                                          double p2,
                                          double p3) {
  return -3 * p0 * (1 - t) * (1 - t) +  //
         p1 * (3 * (1 - t) * (1 - t) - 6 * (1 - t) * t) +
         p2 * (6 * (1 - t) * t - 3 * t * t) +  //
         3 * p3 * t * t;
}

Point LinearPathComponent::solve(double time) const {
  return {
      LinearSolve(time, p1.x, p2.x),  // x
      LinearSolve(time, p1.y, p2.y),  // y
  };
}

Point QuadraticPathComponent::solve(double time) const {
  return {
      QuadraticSolve(time, p1.x, cp.x, p2.x),  // x
      QuadraticSolve(time, p1.y, cp.y, p2.y),  // y
  };
}

Point QuadraticPathComponent::solveDerivative(double time) const {
  return {
      QuadraticSolveDerivative(time, p1.x, cp.x, p2.x),  // x
      QuadraticSolveDerivative(time, p1.y, cp.y, p2.y),  // y
  };
}

Point CubicPathComponent::solve(double time) const {
  return {
      CubicSolve(time, p1.x, cp1.x, cp2.y, p2.x),  // x
      CubicSolve(time, p1.y, cp1.y, cp2.y, p2.y),  // y
  };
}

Point CubicPathComponent::solveDerivative(double time) const {
  return {
      CubicSolveDerivative(time, p1.x, cp1.x, cp2.y, p2.x),  // x
      CubicSolveDerivative(time, p1.y, cp1.y, cp2.y, p2.y),  // y
  };
}

}  // namespace geom
}  // namespace rl
