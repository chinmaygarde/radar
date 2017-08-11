/*
 *  This source file is part of the Radar project.
 *  Licensed under the MIT License. See LICENSE file for details.
 */

#include <TestRunner/TestRunner.h>
#include <Typography/AttributedStringBuilder.h>
#include <Typography/Typesetter.h>
#include <Typography/TypographyContext.h>

TEST(TypesetterTest, SimpleTypesetter) {
  rl::type::AttributedStringBuilder builder;
  std::string hello("Hello");
  builder.appendText(hello);
  auto attributedString = builder.attributedString();
  ASSERT_TRUE(attributedString.isValid());
  rl::type::Typesetter typesetter(attributedString);
  ASSERT_TRUE(typesetter.isValid());
  ASSERT_EQ(typesetter.runs().size(), 1u);
  ASSERT_EQ(typesetter.runs()[0].direction(),
            rl::type::TextRun::Direction::LeftToRight);
}

TEST(TypesetterTest, SimpleHebrewTypesetter) {
  rl::type::AttributedStringBuilder builder;
  std::string hello("ציור עסקים מדע מה. צ'ט בקלות הבאים מאמרשיחהצפה של.");
  builder.appendText(hello);
  auto attributedString = builder.attributedString();
  ASSERT_TRUE(attributedString.isValid());
  rl::type::Typesetter typesetter(std::move(attributedString));
  ASSERT_TRUE(typesetter.isValid());
  ASSERT_EQ(typesetter.runs().size(), 1u);
  ASSERT_EQ(typesetter.runs()[0].direction(),
            rl::type::TextRun::Direction::RightToLeft);
}

TEST(TypesetterTest, MixedTypesetter) {
  rl::type::AttributedStringBuilder builder;
  std::string hello(
      "World ציור עסקים מדע מה. צ'ט בקלות הבאים  Hello מאמרשיחהצפה של");
  builder.appendText(hello);
  auto attributedString = builder.attributedString();
  ASSERT_TRUE(attributedString.isValid());
  rl::type::Typesetter typesetter(std::move(attributedString));
  ASSERT_TRUE(typesetter.isValid());
  ASSERT_EQ(typesetter.runs().size(), 4u);
  ASSERT_EQ(typesetter.runs()[0].direction(),
            rl::type::TextRun::Direction::LeftToRight);
  ASSERT_EQ(typesetter.runs()[1].direction(),
            rl::type::TextRun::Direction::RightToLeft);
  ASSERT_EQ(typesetter.runs()[2].direction(),
            rl::type::TextRun::Direction::LeftToRight);
  ASSERT_EQ(typesetter.runs()[3].direction(),
            rl::type::TextRun::Direction::RightToLeft);
}

TEST(TypesetterTest, EmojiTypesetter) {
  rl::type::AttributedStringBuilder builder;
  std::string hello("With 😀 😃 😄 😁 😆 😅 😂 🤣 Emoji");
  builder.appendText(hello);
  auto attributedString = builder.attributedString();
  ASSERT_TRUE(attributedString.isValid());
  rl::type::Typesetter typesetter(std::move(attributedString));
  ASSERT_TRUE(typesetter.isValid());
  ASSERT_EQ(typesetter.runs().size(), 1u);
  ASSERT_EQ(typesetter.runs()[0].direction(),
            rl::type::TextRun::Direction::LeftToRight);
}

TEST(TypesetterTest, TestRunLengths) {
  rl::type::AttributedStringBuilder builder;
  std::string hello("😄");
  builder.appendText(hello);
  auto attributedString = builder.attributedString();
  ASSERT_TRUE(attributedString.isValid());
  rl::type::Typesetter typesetter(attributedString);
  ASSERT_TRUE(typesetter.isValid());
  ASSERT_EQ(typesetter.runs().size(), 1u);
  ASSERT_EQ(typesetter.runs()[0].direction(),
            rl::type::TextRun::Direction::LeftToRight);
  ASSERT_EQ(typesetter.runs()[0].range().start, 0);
  ASSERT_EQ(typesetter.runs()[0].range().length, 2);
}
