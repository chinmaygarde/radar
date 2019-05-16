/*
 *  This source file is part of the Radar project.
 *  Licensed under the MIT License. See LICENSE file for details.
 */

#include <Core/Message.h>
#include <Geometry/Path.h>
#include <Geometry/PathBuilder.h>
#include <TestRunner/TestRunner.h>

TEST(PathTest, SimplePath) {
  rl::geom::Path path;

  path.addLinearComponent({0, 0}, {100, 100})
      .addQuadraticComponent({100, 100}, {200, 200}, {300, 300})
      .addCubicComponent({300, 300}, {400, 400}, {500, 500}, {600, 600});

  ASSERT_EQ(path.componentCount(), 3u);

  path.enumerateComponents(
      [](size_t index, const rl::geom::LinearPathComponent& linear) {
        rl::geom::Point p1(0, 0);
        rl::geom::Point p2(100, 100);
        ASSERT_EQ(index, 0u);
        ASSERT_EQ(linear.p1, p1);
        ASSERT_EQ(linear.p2, p2);
      },
      [](size_t index, const rl::geom::QuadraticPathComponent& quad) {
        rl::geom::Point p1(100, 100);
        rl::geom::Point cp(200, 200);
        rl::geom::Point p2(300, 300);
        ASSERT_EQ(index, 1u);
        ASSERT_EQ(quad.p1, p1);
        ASSERT_EQ(quad.cp, cp);
        ASSERT_EQ(quad.p2, p2);
      },
      [](size_t index, const rl::geom::CubicPathComponent& cubic) {
        rl::geom::Point p1(300, 300);
        rl::geom::Point cp1(400, 400);
        rl::geom::Point cp2(500, 500);
        rl::geom::Point p2(600, 600);
        ASSERT_EQ(index, 2u);
        ASSERT_EQ(cubic.p1, p1);
        ASSERT_EQ(cubic.cp1, cp1);
        ASSERT_EQ(cubic.cp2, cp2);
        ASSERT_EQ(cubic.p2, p2);
      });
}

TEST(PathTest, PathEncodeDecode) {
  rl::core::Message message;

  {
    rl::geom::Path path;

    path.addLinearComponent({0, 0}, {100, 100})
        .addQuadraticComponent({100, 100}, {200, 200}, {300, 300})
        .addCubicComponent({300, 300}, {400, 400}, {500, 500}, {600, 600});

    ASSERT_TRUE(message.encode(path));
  }

  rl::geom::Path decoded;

  ASSERT_TRUE(message.decode(decoded, nullptr));

  ASSERT_EQ(decoded.componentCount(), 3u);

  decoded.enumerateComponents(
      [](size_t index, const rl::geom::LinearPathComponent& linear) {
        rl::geom::Point p1(0, 0);
        rl::geom::Point p2(100, 100);
        ASSERT_EQ(index, 0u);
        ASSERT_EQ(linear.p1, p1);
        ASSERT_EQ(linear.p2, p2);
      },
      [](size_t index, const rl::geom::QuadraticPathComponent& quad) {
        rl::geom::Point p1(100, 100);
        rl::geom::Point cp(200, 200);
        rl::geom::Point p2(300, 300);
        ASSERT_EQ(index, 1u);
        ASSERT_EQ(quad.p1, p1);
        ASSERT_EQ(quad.cp, cp);
        ASSERT_EQ(quad.p2, p2);
      },
      [](size_t index, const rl::geom::CubicPathComponent& cubic) {
        rl::geom::Point p1(300, 300);
        rl::geom::Point cp1(400, 400);
        rl::geom::Point cp2(500, 500);
        rl::geom::Point p2(600, 600);
        ASSERT_EQ(index, 2u);
        ASSERT_EQ(cubic.p1, p1);
        ASSERT_EQ(cubic.cp1, cp1);
        ASSERT_EQ(cubic.cp2, cp2);
        ASSERT_EQ(cubic.p2, p2);
      });
}

TEST(PathTest, BoundingBoxCubic) {
  rl::geom::Path path;
  path.addCubicComponent({120, 160}, {25, 200}, {220, 260}, {220, 40});
  auto box = path.boundingBox();
  rl::geom::Rect expected(0, 0, 220, 198.862);
  ASSERT_RECT_NEAR(box, expected);
}

TEST(PathTest, BoundingBoxOfCompositePathIsCorrect) {
  rl::geom::PathBuilder builder;
  builder.addRoundedRect({{10, 10}, {300, 300}}, {50, 50, 50, 50});
  auto path = builder.path();
  auto actual = path.boundingBox();
  rl::geom::Rect expected(0, 0, 310, 310);
  ASSERT_RECT_NEAR(actual, expected);
}
