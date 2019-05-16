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
  ASSERT_EQ(typesetter.runs().runs().size(), 1u);
  ASSERT_EQ(typesetter.runs().runs()[0].direction(),
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
  ASSERT_EQ(typesetter.runs().runs().size(), 9u);
  ASSERT_EQ(typesetter.runs().runs()[0].direction(),
            rl::type::TextRun::Direction::RightToLeft);
  ASSERT_EQ(typesetter.runs().runs()[1].direction(),
            rl::type::TextRun::Direction::RightToLeft);
  ASSERT_EQ(typesetter.runs().runs()[2].direction(),
            rl::type::TextRun::Direction::RightToLeft);
  ASSERT_EQ(typesetter.runs().runs()[3].direction(),
            rl::type::TextRun::Direction::RightToLeft);
  ASSERT_EQ(typesetter.runs().runs()[4].direction(),
            rl::type::TextRun::Direction::RightToLeft);
  ASSERT_EQ(typesetter.runs().runs()[5].direction(),
            rl::type::TextRun::Direction::RightToLeft);
  ASSERT_EQ(typesetter.runs().runs()[6].direction(),
            rl::type::TextRun::Direction::RightToLeft);
  ASSERT_EQ(typesetter.runs().runs()[7].direction(),
            rl::type::TextRun::Direction::RightToLeft);
  ASSERT_EQ(typesetter.runs().runs()[8].direction(),
            rl::type::TextRun::Direction::RightToLeft);
}

TEST(TypesetterTest, MixedTypesetter) {
  rl::type::AttributedStringBuilder builder;
  std::string hello(
      "World ציור עסקים מדע מה. צ'ט בקלות הבאים Hello מאמרשיחהצפה של");
  builder.appendText(hello);
  auto attributedString = builder.attributedString();
  ASSERT_TRUE(attributedString.isValid());
  rl::type::Typesetter typesetter(std::move(attributedString));
  ASSERT_TRUE(typesetter.isValid());
  ASSERT_EQ(typesetter.runs().runs().size(), 12u);
  ASSERT_EQ(typesetter.runs().runs()[0].direction(),
            rl::type::TextRun::Direction::LeftToRight);
  ASSERT_EQ(typesetter.runs().runs()[0].range().length, 6u);  // "World "
  ASSERT_EQ(typesetter.runs().runs()[1].direction(),
            rl::type::TextRun::Direction::RightToLeft);  //  "הבאים "
  ASSERT_EQ(typesetter.runs().runs()[2].direction(),
            rl::type::TextRun::Direction::RightToLeft);  //  "בקלות "
  ASSERT_EQ(typesetter.runs().runs()[3].direction(),
            rl::type::TextRun::Direction::RightToLeft);  //  "צ'ט "
  ASSERT_EQ(typesetter.runs().runs()[4].direction(),
            rl::type::TextRun::Direction::RightToLeft);
  ASSERT_EQ(typesetter.runs().runs()[5].direction(),
            rl::type::TextRun::Direction::RightToLeft);
  ASSERT_EQ(typesetter.runs().runs()[6].direction(),
            rl::type::TextRun::Direction::RightToLeft);
  ASSERT_EQ(typesetter.runs().runs()[7].direction(),
            rl::type::TextRun::Direction::RightToLeft);
  ASSERT_EQ(typesetter.runs().runs()[8].direction(),
            rl::type::TextRun::Direction::LeftToRight);  // <whitespace>
  ASSERT_EQ(typesetter.runs().runs()[9].direction(),
            rl::type::TextRun::Direction::LeftToRight);  // "Hello "
  ASSERT_EQ(typesetter.runs().runs()[10].direction(),
            rl::type::TextRun::Direction::RightToLeft);
  ASSERT_EQ(typesetter.runs().runs()[11].direction(),
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
  ASSERT_EQ(typesetter.runs().runs().size(), 10u);
  ASSERT_EQ(typesetter.runs().runs()[0].direction(),
            rl::type::TextRun::Direction::LeftToRight);
  ASSERT_EQ(typesetter.runs().runs()[1].direction(),
            rl::type::TextRun::Direction::LeftToRight);
  ASSERT_EQ(typesetter.runs().runs()[2].direction(),
            rl::type::TextRun::Direction::LeftToRight);
  ASSERT_EQ(typesetter.runs().runs()[3].direction(),
            rl::type::TextRun::Direction::LeftToRight);
  ASSERT_EQ(typesetter.runs().runs()[4].direction(),
            rl::type::TextRun::Direction::LeftToRight);
  ASSERT_EQ(typesetter.runs().runs()[5].direction(),
            rl::type::TextRun::Direction::LeftToRight);
  ASSERT_EQ(typesetter.runs().runs()[6].direction(),
            rl::type::TextRun::Direction::LeftToRight);
  ASSERT_EQ(typesetter.runs().runs()[7].direction(),
            rl::type::TextRun::Direction::LeftToRight);
  ASSERT_EQ(typesetter.runs().runs()[8].direction(),
            rl::type::TextRun::Direction::LeftToRight);
  ASSERT_EQ(typesetter.runs().runs()[9].direction(),
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
  ASSERT_EQ(typesetter.runs().runs().size(), 1u);
  ASSERT_EQ(typesetter.runs().runs()[0].direction(),
            rl::type::TextRun::Direction::LeftToRight);
  ASSERT_EQ(typesetter.runs().runs()[0].range().start, 0u);
  ASSERT_EQ(typesetter.runs().runs()[0].range().length, 2u);
}

TEST(TypesetterTest, SimpleTypesetterCreateShapedRuns) {
  rl::type::AttributedStringBuilder builder;
  std::string hello("Hello World");
  builder.pushFontDescriptor({"Roboto-Regular", 22.0}).appendText(hello);
  auto attributedString = builder.attributedString();
  ASSERT_TRUE(attributedString.isValid());
  rl::type::Typesetter typesetter(attributedString);
  ASSERT_TRUE(typesetter.isValid());
  rl::type::FontLibrary library;
  ASSERT_TRUE(library.registerFont(rl::core::URI{"Roboto-Regular.ttf"}, 0));
  auto shapedRuns = typesetter.createShapedRuns(library);
  ASSERT_EQ(shapedRuns.size(), 2u);
  ASSERT_EQ(shapedRuns[0].glyphCount(), 6u);
  ASSERT_EQ(shapedRuns[1].glyphCount(), 5u);
}
