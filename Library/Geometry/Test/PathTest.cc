// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <TestRunner/TestRunner.h>
#include <Geometry/Path.h>

TEST(PathTest, SimplePath) {
  rl::geom::Path path;

  path.addLinearComponent({0, 0}, {100, 100})
      .addQuadraticComponent({100, 100}, {200, 200}, {300, 300})
      .addCubicComponent({300, 300}, {400, 400}, {500, 500}, {600, 600});

  ASSERT_EQ(path.componentCount(), 3);

  path.enumerateComponents(
      [](size_t index, const rl::geom::LinearPathComponent& linear) {
        rl::geom::Point p1(0, 0);
        rl::geom::Point p2(100, 100);
        ASSERT_EQ(index, 0);
        ASSERT_EQ(linear.p1, p1);
        ASSERT_EQ(linear.p2, p2);
      },
      [](size_t index, const rl::geom::QuadraticPathComponent& quad) {
        rl::geom::Point p1(100, 100);
        rl::geom::Point cp(200, 200);
        rl::geom::Point p2(300, 300);
        ASSERT_EQ(index, 1);
        ASSERT_EQ(quad.p1, p1);
        ASSERT_EQ(quad.cp, cp);
        ASSERT_EQ(quad.p2, p2);
      },
      [](size_t index, const rl::geom::CubicPathComponent& cubic) {
        rl::geom::Point p1(300, 300);
        rl::geom::Point cp1(400, 400);
        rl::geom::Point cp2(500, 500);
        rl::geom::Point p2(600, 600);
        ASSERT_EQ(index, 2);
        ASSERT_EQ(cubic.p1, p1);
        ASSERT_EQ(cubic.cp1, cp1);
        ASSERT_EQ(cubic.cp2, cp2);
        ASSERT_EQ(cubic.p2, p2);
      });
}
