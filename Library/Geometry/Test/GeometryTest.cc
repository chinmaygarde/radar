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

std::ostream& operator<<(std::ostream& out, const rl::geom::Quaternion& q) {
  out << "(" << q.x << ", " << q.y << ", " << q.z << ", " << q.w << ")";
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

::testing::AssertionResult QuaternionNear(rl::geom::Quaternion a,
                                          rl::geom::Quaternion b) {
  auto equal = Near(a.x, b.x)     //
               && Near(a.y, b.y)  //
               && Near(a.z, b.z)  //
               && Near(a.w, b.w)  //
      ;

  return equal ? ::testing::AssertionSuccess()
               : ::testing::AssertionFailure() << "Quaternions are not equal.";
}

#define ASSERT_MATRIX_NEAR(a, b) ASSERT_PRED2(&::MatrixNear, a, b)
#define ASSERT_QUATERNION_NEAR(a, b) ASSERT_PRED2(&::QuaternionNear, a, b)

TEST(GeometryTest, RotationMatrix) {
  auto rotation = rl::geom::Matrix::RotationZ(M_PI_4);
  auto expect = rl::geom::Matrix{0.707,  0.707, 0, 0,  //
                                 -0.707, 0.707, 0, 0,  //
                                 0,      0,     1, 0,  //
                                 0,      0,     0, 1};
  ASSERT_MATRIX_NEAR(rotation, expect);
}

TEST(GeometryTest, InvertMultMatrix) {
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
  ASSERT_MATRIX_NEAR(rotation * invert, rl::geom::Matrix{});
}

TEST(GeometryTest, DeterminantTest) {
  auto matrix =
      rl::geom::Matrix{3, 4, 14, 155, 2, 1, 3, 4, 2, 3, 2, 1, 1, 2, 4, 2};
  ASSERT_EQ(matrix.determinant(), -1889);
}

TEST(GeometryTest, InvertMatrix) {
  auto inverted = rl::geom::Matrix{10,  -9,  -12, 8,   //
                                   7,   -12, 11,  22,  //
                                   -10, 10,  3,   6,   //
                                   -2,  22,  2,   1}
                      .invert();

  auto result = rl::geom::Matrix{
      438.0 / 85123.0,   1751.0 / 85123.0, -7783.0 / 85123.0, 4672.0 / 85123.0,
      393.0 / 85123.0,   -178.0 / 85123.0, -570.0 / 85123.0,  4192 / 85123.0,
      -5230.0 / 85123.0, 2802.0 / 85123.0, -3461.0 / 85123.0, 962.0 / 85123.0,
      2690.0 / 85123.0,  1814.0 / 85123.0, 3896.0 / 85123.0,  319.0 / 85123.0};

  ASSERT_MATRIX_NEAR(inverted, result);
}

TEST(GeometryTest, TestDecomposition) {
  auto rotated = rl::geom::Matrix::RotationZ(M_PI_4);

  auto result = rotated.decompose();

  ASSERT_TRUE(result.first);

  rl::geom::Matrix::Decomposition res = result.second;

  auto quaternion = rl::geom::Quaternion{{0.0, 0.0, 1.0}, M_PI_4};
  ASSERT_QUATERNION_NEAR(res.rotation, quaternion);
}

TEST(GeometryTest, TestDecomposition2) {
  auto rotated = rl::geom::Matrix::RotationZ(M_PI_4);
  auto scaled = rl::geom::Matrix::Scale({2.0, 3.0, 1.0});
  auto translated = rl::geom::Matrix::Translation({-200, 750, 20});

  auto result = (translated * rotated * scaled).decompose();

  ASSERT_TRUE(result.first);

  rl::geom::Matrix::Decomposition res = result.second;

  auto quaternion = rl::geom::Quaternion{{0.0, 0.0, 1.0}, M_PI_4};

  ASSERT_QUATERNION_NEAR(res.rotation, quaternion);

  ASSERT_DOUBLE_EQ(res.translation.x, -200);
  ASSERT_DOUBLE_EQ(res.translation.y, 750);
  ASSERT_DOUBLE_EQ(res.translation.z, 20);

  ASSERT_DOUBLE_EQ(res.scale.x, 2);
  ASSERT_DOUBLE_EQ(res.scale.y, 3);
  ASSERT_DOUBLE_EQ(res.scale.z, 1);
}

TEST(GeometryTest, TestRecomposition) {
  /*
   *  Decomposition.
   */
  auto rotated = rl::geom::Matrix::RotationZ(M_PI_4);

  auto result = rotated.decompose();

  ASSERT_TRUE(result.first);

  rl::geom::Matrix::Decomposition res = result.second;

  auto quaternion = rl::geom::Quaternion{{0.0, 0.0, 1.0}, M_PI_4};

  ASSERT_QUATERNION_NEAR(res.rotation, quaternion);

  /*
   *  Recomposition.
   */
  ASSERT_MATRIX_NEAR(rotated, rl::geom::Matrix{res});
}

TEST(GeometryTest, TestRecomposition2) {
  auto matrix = rl::geom::Matrix::Translation({100, 100, 100}) *
                rl::geom::Matrix::RotationZ(M_PI_4) *
                rl::geom::Matrix::Scale({2.0, 2.0, 2.0});

  auto result = matrix.decompose();

  ASSERT_TRUE(result.first);

  ASSERT_MATRIX_NEAR(matrix, rl::geom::Matrix{result.second});
}
