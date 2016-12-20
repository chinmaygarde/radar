// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <TestRunner/TestRunner.h>
#include <Core/Core.h>
#include <thread>
#include "SVGXFormParser/SVGXformDriver.h"

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

TEST(SVGXFormParserTest, Invalid) {
  auto invalid = "This is invalid!";
  rl::SVGXFormDriver driver;
  auto result = driver.parse(invalid);
  ASSERT_EQ(result, rl::SVGXFormDriver::ParserResult::SyntaxError);
  rl::geom::Matrix identity;
  ASSERT_MATRIX_NEAR(driver.transformation(), identity);
}

TEST(SVGXFormParserTest, Translate_1) {
  auto text = "translate(-10)";
  rl::SVGXFormDriver driver;
  auto result = driver.parse(text);
  ASSERT_EQ(result, rl::SVGXFormDriver::ParserResult::Success);
  rl::geom::Matrix expected = rl::geom::Matrix::Translation({-10, 0, 0});
  ASSERT_MATRIX_NEAR(driver.transformation(), expected);
}

TEST(SVGXFormParserTest, Translate_2) {
  auto text = "translate(-10,-20)";
  rl::SVGXFormDriver driver;
  auto result = driver.parse(text);
  ASSERT_EQ(result, rl::SVGXFormDriver::ParserResult::Success);
  rl::geom::Matrix expected = rl::geom::Matrix::Translation({-10, -20, 0});
  ASSERT_MATRIX_NEAR(driver.transformation(), expected);
}

TEST(SVGXFormParserTest, Scale_1) {
  auto text = " scale(2) ";
  rl::SVGXFormDriver driver;
  auto result = driver.parse(text);
  ASSERT_EQ(result, rl::SVGXFormDriver::ParserResult::Success);
  rl::geom::Matrix expected = rl::geom::Matrix::Scale({2.0, 2.0, 1.0});
  ASSERT_MATRIX_NEAR(driver.transformation(), expected);
}

TEST(SVGXFormParserTest, Scale_2) {
  auto text = " scale(2, 23) ";
  rl::SVGXFormDriver driver;
  auto result = driver.parse(text);
  ASSERT_EQ(result, rl::SVGXFormDriver::ParserResult::Success);
  rl::geom::Matrix expected = rl::geom::Matrix::Scale({2.0, 23.0, 1.0});
  ASSERT_MATRIX_NEAR(driver.transformation(), expected);
}

TEST(SVGXFormParserTest, Rotate_1) {
  auto text = "rotate(45)";
  rl::SVGXFormDriver driver;
  auto result = driver.parse(text);
  ASSERT_EQ(result, rl::SVGXFormDriver::ParserResult::Success);
  rl::geom::Matrix expected = rl::geom::Matrix::RotationZ(45.0 * M_PI / 180.0);
  ASSERT_MATRIX_NEAR(driver.transformation(), expected);
}

TEST(SVGXFormParserTest, Rotate_2) {
  auto text = "rotate(45 200 200)";
  rl::SVGXFormDriver driver;
  auto result = driver.parse(text);
  ASSERT_EQ(result, rl::SVGXFormDriver::ParserResult::Success);
  rl::geom::Matrix expected = rl::geom::Matrix::Translation({200, 200, 0.0}) *
                              rl::geom::Matrix::RotationZ(45.0 * M_PI / 180.0) *
                              rl::geom::Matrix::Translation({-200, -200, 0.0});
  ASSERT_MATRIX_NEAR(driver.transformation(), expected);
}

TEST(SVGXFormParserTest, SkewX) {
  auto text = "skewX(45)";
  rl::SVGXFormDriver driver;
  auto result = driver.parse(text);
  ASSERT_EQ(result, rl::SVGXFormDriver::ParserResult::Success);
}

TEST(SVGXFormParserTest, SkewY) {
  auto text = "skewY(45)";
  rl::SVGXFormDriver driver;
  auto result = driver.parse(text);
  ASSERT_EQ(result, rl::SVGXFormDriver::ParserResult::Success);
}

TEST(SVGXFormParserTest, MultipleChained) {
  auto text =
      "translate(140.500000, 102.000000) rotate(36.000000) "
      "translate(-140.500000, -102.000000)";
  rl::SVGXFormDriver driver;
  auto result = driver.parse(text);
  ASSERT_EQ(result, rl::SVGXFormDriver::ParserResult::Success);
  rl::geom::Matrix expected =
      rl::geom::Matrix::Translation({140.5, 102, 0.0}) *
      rl::geom::Matrix::RotationZ(36.0 * M_PI / 180.0) *
      rl::geom::Matrix::Translation({-140.5, -102, 0.0});
  ASSERT_MATRIX_NEAR(driver.transformation(), expected);
}
