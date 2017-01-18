// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <Geometry/Matrix.h>
#include <TestRunner/TestRunner.h>
#include <ostream>

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

TEST(GeometryTest, QuaternionLerp) {
  auto q1 = rl::geom::Quaternion{{0.0, 0.0, 1.0}, 0.0};
  auto q2 = rl::geom::Quaternion{{0.0, 0.0, 1.0}, M_PI_4};

  auto q3 = q1.slerp(q2, 0.5);

  auto expected = rl::geom::Quaternion{{0.0, 0.0, 1.0}, M_PI_4 / 2.0};

  ASSERT_QUATERNION_NEAR(q3, expected);
}

TEST(GeometryTest, RectWithPoint) {
  auto rect = rl::geom::Rect{};

  auto expected = rl::geom::Rect{};

  ASSERT_RECT_NEAR(rect, expected);

  rect = rect.withPoint({100, 100});
  expected = rl::geom::Rect{0, 0, 100, 100};
  ASSERT_RECT_NEAR(rect, expected);

  rect = rect.withPoint({-11, -12});
  expected = rl::geom::Rect{-11, -12, 111, 112};
  ASSERT_RECT_NEAR(rect, expected);

  rect = rect.withPoint({55, 65});
  expected = rl::geom::Rect{-11, -12, 111, 112};
  ASSERT_RECT_NEAR(rect, expected);

  rect = rect.withPoint({-25, 0});
  expected = rl::geom::Rect{-25, -12, 125, 112};
  ASSERT_RECT_NEAR(rect, expected);

  rect = rect.withPoint({0, -25});
  expected = rl::geom::Rect{-25, -25, 125, 125};
  ASSERT_RECT_NEAR(rect, expected);

  rect = rect.withPoint({125, 135});
  expected = rl::geom::Rect{-25, -25, 150, 160};
  ASSERT_RECT_NEAR(rect, expected);
}
