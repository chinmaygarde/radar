/*
 *  This source file is part of the Radar project.
 *  Licensed under the MIT License. See LICENSE file for details.
 */

#include <TestRunner/TestRunner.h>
#include <thread>
#include "SVGXFormParser/SVGXFormDriver.h"

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
