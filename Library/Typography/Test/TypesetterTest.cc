/*
 *  This source file is part of the Radar project.
 *  Licensed under the MIT License. See LICENSE file for details.
 */

#include <TestRunner/TestRunner.h>
#include <Typography/AttributedStringBuilder.h>
#include <Typography/Typesetter.h>
#include <Typography/TypographyContext.h>

TEST(FramesetterTest, SimpleFramesetterCreate) {
  rl::type::AttributedStringBuilder builder;
  std::string hello("Hello");
  builder.appendText(hello);
  auto attributedString = builder.attributedString();
  ASSERT_TRUE(attributedString.isValid());
  rl::type::Typesetter typesetter(attributedString);
  ASSERT_TRUE(typesetter.isValid());
  rl::type::Typesetter typesetter2(attributedString);
  ASSERT_TRUE(typesetter2.isValid());
  rl::type::Typesetter typesetter3(attributedString);
  ASSERT_TRUE(typesetter3.isValid());
}

TEST(FramesetterTest, SimpleFramesetterPerformFramesetting) {
  rl::type::AttributedStringBuilder builder;
  std::string hello("Hello");
  builder.appendText(hello);
  auto attributedString = builder.attributedString();
  ASSERT_TRUE(attributedString.isValid());
  rl::type::Typesetter typesetter(std::move(attributedString));
  auto frame = typesetter.createTypeFrame({1000, 1000});
  ASSERT_TRUE(frame.isValid());
}
