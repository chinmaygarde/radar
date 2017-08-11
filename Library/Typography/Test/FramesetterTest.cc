/*
 *  This source file is part of the Radar project.
 *  Licensed under the MIT License. See LICENSE file for details.
 */

#include <TestRunner/TestRunner.h>
#include <Typography/AttributedStringBuilder.h>
#include <Typography/Framesetter.h>

TEST(FramesetterTest, SimpleFramesetter) {
  rl::type::AttributedStringBuilder builder;
  std::string hello("Hello");
  builder.appendText(hello);
  auto attributedString = builder.attributedString();
  ASSERT_TRUE(attributedString.isValid());

  rl::type::Framesetter framesetter(std::move(attributedString), {100, 100});
  ASSERT_TRUE(framesetter.isValid());
}
