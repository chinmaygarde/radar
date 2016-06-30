// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <TestRunner/TestRunner.h>
#include <Geometry/Matrix.h>
#include <ostream>

namespace std {
std::ostream& operator<<(std::ostream& out, const rl::geom::Matrix& m) {
  out << "(" << m.toString() << ")";
  return out;
}
}  // namespace std

static bool Near(double a, double b) {
  static const double epsilon = 1e-3;
  return (a > (b - epsilon)) && (a < (b + epsilon));
}

::testing::AssertionResult MatrixNear(rl::geom::Matrix a, rl::geom::Matrix b) {
  auto equal = Near(a.m[0], b.m[0])       //
               && Near(a.m[1], b.m[1])    //
               && Near(a.m[2], b.m[2])    //
               && Near(a.m[3], b.m[3])    //
               && Near(a.m[4], b.m[4])    //
               && Near(a.m[5], b.m[5])    //
               && Near(a.m[6], b.m[6])    //
               && Near(a.m[7], b.m[7])    //
               && Near(a.m[8], b.m[8])    //
               && Near(a.m[9], b.m[9])    //
               && Near(a.m[10], b.m[10])  //
               && Near(a.m[11], b.m[11])  //
               && Near(a.m[12], b.m[12])  //
               && Near(a.m[13], b.m[13])  //
               && Near(a.m[14], b.m[14])  //
               && Near(a.m[15], b.m[15]);

  return equal ? ::testing::AssertionSuccess()
               : ::testing::AssertionFailure() << "Matrixes are not equal.";
}

#define ASSERT_MATRIX_NEAR(a, b) ASSERT_PRED2(&::MatrixNear, a, b)

TEST(GeometryTest, RotationMatrix) {
  auto rotation = rl::geom::Matrix::RotationZ(M_PI_4);
  auto expect = rl::geom::Matrix{0.707,  0.707, 0, 0,  //
                                 -0.707, 0.707, 0, 0,  //
                                 0,      0,     1, 0,  //
                                 0,      0,     0, 1};
  ASSERT_MATRIX_NEAR(rotation, expect);
}

TEST(GeometryTest, InvertMatrix) {
  auto rotation = rl::geom::Matrix::RotationZ(M_PI_4);
  auto invert = rotation.invert();
  auto expect = rl::geom::Matrix{0.707, -0.707, 0, 0,  //
                                 0.707, 0.707,  0, 0,  //
                                 0,     0,      1, 0,  //
                                 0,     0,      0, 1};
  ASSERT_MATRIX_NEAR(invert, expect);
}

TEST(GeometryTest, MutliplicationMatrix) {
  auto rotation = rl::geom::Matrix::RotationZ(M_PI_4);
  auto invert = rotation.invert();
  ASSERT_MATRIX_NEAR(rotation * invert, rl::geom::MatrixIdentity);
}
