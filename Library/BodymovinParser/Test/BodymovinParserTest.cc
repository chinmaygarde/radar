/*
 *  This source file is part of the Radar project.
 *  Licensed under the MIT License. See LICENSE file for details.
 */

#include <BodymovinParser/Animation.h>
#include <Core/FileMapping.h>
#include <TestRunner/TestRunner.h>

TEST(BodymovinParserTest, Invalid) {
  rl::core::FileMapping mapping;
  auto animation = rl::bodymovin::Animation::ParseManifest(mapping);
  ASSERT_FALSE(animation);
}

TEST(BodymovinParserTest, Valid) {
  rl::core::FileMapping mapping(rl::core::URI{"Watermelon.json"});
  auto animation = rl::bodymovin::Animation::Animation::ParseManifest(mapping);
  ASSERT_TRUE(animation);
  ASSERT_EQ(animation->layers().size(), 12);
  rl::geom::Size size(100, 100);
  ASSERT_SIZE_NEAR(animation->compositionSize(), size);
  ASSERT_EQ(animation->bodymovinVersion(), "4.4.26");
  ASSERT_EQ(animation->layers()[3]->name(), "W3");
  ASSERT_EQ(animation->layers()[3]->parentIndex(), 10);
  ASSERT_EQ(animation->layers()[3]->startTime(), -18);
  ASSERT_EQ(animation->layers()[3]->inPoint(), 312);
  ASSERT_EQ(animation->layers()[3]->outPoint(), 354);
  ASSERT_EQ(animation->layers()[1]->name(), "W1");
  auto shapeLayer = reinterpret_cast<rl::bodymovin::ShapeLayer*>(
      animation->layers()[1].get());
  ASSERT_EQ(shapeLayer->shapeItems().size(), 1);
  auto groupShape = reinterpret_cast<rl::bodymovin::GroupShape*>(
      shapeLayer->shapeItems()[0].get());
  ASSERT_EQ(groupShape->shapeItems().size(), 3);
}
