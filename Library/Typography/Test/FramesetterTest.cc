/*
 *  This source file is part of the Radar project.
 *  Licensed under the MIT License. See LICENSE file for details.
 */

#include <TestRunner/TestRunner.h>
#include <Typography/AttributedStringBuilder.h>
#include <Typography/Framesetter.h>
#include <Typography/TypographyContext.h>

TEST(FramesetterTest, SimpleFramesetter) {
  rl::type::AttributedStringBuilder builder;
  std::string hello("Hello");
  builder.appendText(hello);
  auto attributedString = builder.attributedString();
  ASSERT_TRUE(attributedString.isValid());
  rl::type::Framesetter framesetter(attributedString);
  ASSERT_TRUE(framesetter.isValid());
  rl::type::Framesetter framesetter2(attributedString);
  ASSERT_TRUE(framesetter2.isValid());
  rl::type::Framesetter framesetter3(attributedString);
  ASSERT_TRUE(framesetter3.isValid());
}
