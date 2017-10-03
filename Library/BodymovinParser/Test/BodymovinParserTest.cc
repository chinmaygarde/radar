/*
 *  This source file is part of the Radar project.
 *  Licensed under the MIT License. See LICENSE file for details.
 */

#include <BodymovinParser/Animation.h>
#include <Core/FileMapping.h>
#include <TestRunner/TestRunner.h>

TEST(BodymovinParserTest, Invalid) {
  rl::core::FileMapping mapping;
  auto invalid = rl::bodymovin::Animation::Create(mapping);
  ASSERT_FALSE(invalid);
}

TEST(BodymovinParserTest, Valid) {
  rl::core::FileMapping mapping(rl::core::URI{"Watermelon.json"});
  auto valid = rl::bodymovin::Animation::Create(mapping);
  ASSERT_TRUE(valid);
}
