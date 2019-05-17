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
  ASSERT_EQ(animation->layers().size(), 12u);
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
  ASSERT_EQ(shapeLayer->shapeItems().size(), 1u);
  auto groupShape = reinterpret_cast<rl::bodymovin::GroupShape*>(
      shapeLayer->shapeItems()[0].get());
  ASSERT_EQ(groupShape->shapeItems().size(), 3u);
}

TEST(BodymovinParserTest, CanParseAllFixtures) {
  std::vector<std::string> fixtures = {
    "A.json",
    "B.json",
    "C.json",
    "D.json",
    "E.json",
    "F.json",
#if 0
      "G.json",
      "H.json",
      "I.json",
      "J.json",
      "K.json",
      "L.json",
      "M.json",
      "N.json",
      "O.json",
      "P.json",
      "Q.json",
      "R.json",
      "S.json",
      "T.json",
      "U.json",
      "V.json",
      "W.json",
      "X.json",
      "Y.json",
      "Z.json",
      "9squares-AlBoardman.json",
      "Apostrophe.json",
      "BlinkingCursor.json",
      "Colon.json",
      "Comma.json",
      "HamburgerArrow.json",
      "IconTransitions.json",
      "LottieLogo1.json",
      "LottieLogo1_masked.json",
      "LottieLogo2.json",
      "MotionCorpse-Jrcanest.json",
      "PinJump.json",
      "Switch.json",
      "Switch_States.json",
      "TwitterHeart.json",
      "vcTransition1.json",
      "vcTransition2.json",
      "Watermelon.json",
#endif
  };

  for (const auto& file : fixtures) {
    RL_LOG("Parsing '%s'...", file.c_str());
    rl::core::FileMapping mapping(rl::core::URI{file});
    auto animation =
        rl::bodymovin::Animation::Animation::ParseManifest(mapping);
    ASSERT_TRUE(animation);
  }
}
