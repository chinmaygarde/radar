// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <TestRunner/TestRunner.h>
#include <Core/Core.h>
#include <thread>
#include "SVGPathDriver.h"

TEST(SVGPathParserTest, Invalid) {
  auto invalid = "This is invalid!";
  rl::SVGPathDriver driver;
  auto result = driver.parse(invalid);
  ASSERT_EQ(result, rl::SVGPathDriver::ParserResult::SyntaxError);
}

TEST(SVGPathParserTest, Triangle01) {
  auto path = "M 100 100 L 300 100 L 200 300 z";
  rl::SVGPathDriver driver;
  auto result = driver.parse(path);
  ASSERT_EQ(result, rl::SVGPathDriver::ParserResult::Success);
  auto finalPath = driver.pathBuilder().path();
  ASSERT_EQ(finalPath.componentCount(), 3);
  finalPath.enumerateComponents(
      [](size_t index, const rl::geom::LinearPathComponent& linear) {
        switch (index) {
          case 0: {
            rl::geom::LinearPathComponent check({100, 100}, {300, 100});
            ASSERT_EQ(check, linear);
          } break;
          case 1: {
            rl::geom::LinearPathComponent check({300, 100}, {200, 300});
            ASSERT_EQ(check, linear);
          } break;
          case 2: {
            rl::geom::LinearPathComponent check({200, 300}, {100, 100});
            ASSERT_EQ(check, linear);
          } break;
          default:
            ASSERT_TRUE(false);
            break;
        }
      },
      [](size_t index, const rl::geom::QuadraticPathComponent& quad) {
        ASSERT_TRUE(false);
      },
      [](size_t index, const rl::geom::CubicPathComponent& cubic) {
        ASSERT_TRUE(false);
      });
}

TEST(SVGPathParserTest, MoreLines) {
  auto path = "M 100 100 L 200 300 L 450 790 l 25 25 z";
  rl::SVGPathDriver driver;
  auto result = driver.parse(path);
  ASSERT_EQ(result, rl::SVGPathDriver::ParserResult::Success);
  auto finalPath = driver.pathBuilder().path();
  ASSERT_EQ(finalPath.componentCount(), 4);
  finalPath.enumerateComponents(
      [](size_t index, const rl::geom::LinearPathComponent& linear) {
        switch (index) {
          case 0: {
            rl::geom::LinearPathComponent check({100, 100}, {200, 300});
            ASSERT_EQ(check, linear);
          } break;
          case 1: {
            rl::geom::LinearPathComponent check({200, 300}, {450, 790});
            ASSERT_EQ(check, linear);
          } break;
          case 2: {
            rl::geom::LinearPathComponent check({450, 790}, {475, 815});
            ASSERT_EQ(check, linear);
          } break;
          case 3: {
            rl::geom::LinearPathComponent check({475, 815}, {100, 100});
            ASSERT_EQ(check, linear);
          } break;
          default:
            ASSERT_TRUE(false);
            break;
        }
      },
      [](size_t index, const rl::geom::QuadraticPathComponent& quad) {
        ASSERT_TRUE(false);
      },
      [](size_t index, const rl::geom::CubicPathComponent& cubic) {
        ASSERT_TRUE(false);
      });
}

TEST(SVGPathParserTest, MoreLinesWithNoCommandRepetition) {
  auto path = "M 100 100 L 200 300 450 790 25 25 z";
  rl::SVGPathDriver driver;
  auto result = driver.parse(path);
  ASSERT_EQ(result, rl::SVGPathDriver::ParserResult::Success);
  auto finalPath = driver.pathBuilder().path();
  ASSERT_EQ(finalPath.componentCount(), 4);
  finalPath.enumerateComponents(
      [](size_t index, const rl::geom::LinearPathComponent& linear) {
        switch (index) {
          case 0: {
            rl::geom::LinearPathComponent check({100, 100}, {200, 300});
            ASSERT_EQ(check, linear);
          } break;
          case 1: {
            rl::geom::LinearPathComponent check({200, 300}, {450, 790});
            ASSERT_EQ(check, linear);
          } break;
          case 2: {
            rl::geom::LinearPathComponent check({450, 790}, {25, 25});
            ASSERT_EQ(check, linear);
          } break;
          case 3: {
            rl::geom::LinearPathComponent check({25, 25}, {100, 100});
            ASSERT_EQ(check, linear);
          } break;
          default:
            ASSERT_TRUE(false);
            break;
        }
      },
      [](size_t index, const rl::geom::QuadraticPathComponent& quad) {
        ASSERT_TRUE(false);
      },
      [](size_t index, const rl::geom::CubicPathComponent& cubic) {
        ASSERT_TRUE(false);
      });
}

TEST(SVGPathParserTest, Cubic01) {
  auto path = R"(M100,200 C100,100 250,100 250,200
  S400,300 400,200)";
  rl::SVGPathDriver driver;
  auto result = driver.parse(path);
  ASSERT_EQ(result, rl::SVGPathDriver::ParserResult::Success);
  auto finalPath = driver.pathBuilder().path();
  ASSERT_EQ(finalPath.componentCount(), 2);
  finalPath.enumerateComponents(
      [](size_t index, const rl::geom::LinearPathComponent& linear) {
        switch (index) {
          default:
            ASSERT_TRUE(false);
            break;
        }
      },
      [](size_t index, const rl::geom::QuadraticPathComponent& quad) {
        switch (index) {
          default:
            ASSERT_TRUE(false);
            break;
        }
      },
      [](size_t index, const rl::geom::CubicPathComponent& cubic) {
        switch (index) {
          case 0: {
            rl::geom::CubicPathComponent expected({100, 200}, {100, 100},
                                                  {250, 100}, {250, 200});
            ASSERT_EQ(cubic, expected);

          } break;
          case 1: {
            rl::geom::CubicPathComponent expected({250, 200}, {250, 300},
                                                  {400, 300}, {400, 200});
            ASSERT_EQ(cubic, expected);
          } break;
          default:
            ASSERT_TRUE(false);
            break;
        }
      });
}

TEST(SVGPathParserTest, Quad01) {
  auto path = R"(M200,300 Q400,50 600,300 T1000,300)";
  rl::SVGPathDriver driver;
  auto result = driver.parse(path);
  ASSERT_EQ(result, rl::SVGPathDriver::ParserResult::Success);
}

TEST(SVGPathParserTest, Quad01_2) {
  auto path = R"(M200,300 L400,50 L600,300
  L800,550 L1000,300)";
  rl::SVGPathDriver driver;
  auto result = driver.parse(path);
  ASSERT_EQ(result, rl::SVGPathDriver::ParserResult::Success);
}

TEST(SVGPathParserTest, Arcs01) {
  auto path = R"(M300,200 h-150 a150,150 0 1,0 150,-150 z)";
  rl::SVGPathDriver driver;
  auto result = driver.parse(path);
  ASSERT_EQ(result, rl::SVGPathDriver::ParserResult::Success);
}

TEST(SVGPathParserTest, Arcs01_2) {
  auto path = R"(M600,350 l 50,-25
  a25,25 -30 0,1 50,-25 l 50,-25
  a25,50 -30 0,1 50,-25 l 50,-25
  a25,75 -30 0,1 50,-25 l 50,-25
  a25,100 -30 0,1 50,-25 l 50,-25)";
  rl::SVGPathDriver driver;
  auto result = driver.parse(path);
  ASSERT_EQ(result, rl::SVGPathDriver::ParserResult::Success);
}
