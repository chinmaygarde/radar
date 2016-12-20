// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <TestRunner/TestRunner.h>
#include <Core/Core.h>
#include <thread>
#include "SVGXFormParser/SVGXformDriver.h"

TEST(SVGXFormParserTest, Invalid) {
  auto invalid = "This is invalid!";
  rl::SVGXFormDriver driver;
  auto result = driver.parse(invalid);
  ASSERT_EQ(result, rl::SVGXFormDriver::ParserResult::SyntaxError);
}

TEST(SVGXFormParserTest, Translate_1) {
  auto text = "translate(-10)";
  rl::SVGXFormDriver driver;
  auto result = driver.parse(text);
  ASSERT_EQ(result, rl::SVGXFormDriver::ParserResult::Success);
}

TEST(SVGXFormParserTest, Translate_2) {
  auto text = "translate(-10,-20)";
  rl::SVGXFormDriver driver;
  auto result = driver.parse(text);
  ASSERT_EQ(result, rl::SVGXFormDriver::ParserResult::Success);
}

TEST(SVGXFormParserTest, Scale_1) {
  auto text = " scale(2) ";
  rl::SVGXFormDriver driver;
  auto result = driver.parse(text);
  ASSERT_EQ(result, rl::SVGXFormDriver::ParserResult::Success);
}

TEST(SVGXFormParserTest, Scale_2) {
  auto text = " scale(2, 23) ";
  rl::SVGXFormDriver driver;
  auto result = driver.parse(text);
  ASSERT_EQ(result, rl::SVGXFormDriver::ParserResult::Success);
}

TEST(SVGXFormParserTest, Rotate_1) {
  auto text = "rotate(45)";
  rl::SVGXFormDriver driver;
  auto result = driver.parse(text);
  ASSERT_EQ(result, rl::SVGXFormDriver::ParserResult::Success);
}

TEST(SVGXFormParserTest, Rotate_2) {
  auto text = "rotate(45 200 200)";
  rl::SVGXFormDriver driver;
  auto result = driver.parse(text);
  ASSERT_EQ(result, rl::SVGXFormDriver::ParserResult::Success);
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
