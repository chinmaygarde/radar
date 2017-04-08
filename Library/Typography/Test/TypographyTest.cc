/*
 *  This source file is part of the Radar project.
 *  Licensed under the MIT License. See LICENSE file for details.
 */

#include <FTLibrary.h>
#include <HBBuffer.h>
#include <TestRunner/TestRunner.h>

TEST(TypographyTest, SimpleFontRegistration) {
  rl::type::FTLibrary library;
  ASSERT_TRUE(library.registerFont(rl::core::URI{"Roboto-Regular.ttf"}));
}

TEST(TypographyTest, AcquireFontNotPresent) {
  rl::type::FTLibrary library;
  auto font = library.fontForTypeface("Roboto-Regular");
  ASSERT_FALSE(font.isValid());
}

TEST(TypographyTest, AcquireFont) {
  rl::type::FTLibrary library;
  ASSERT_TRUE(library.registerFont(rl::core::URI{"Roboto-Regular.ttf"}));
  auto font = library.fontForTypeface("Roboto-Regular");
  ASSERT_TRUE(font.isValid());
}

TEST(TypographyTest, SimpleBuffer) {
  rl::type::FTLibrary library;
  ASSERT_TRUE(library.registerFont(rl::core::URI{"Roboto-Regular.ttf"}));

  auto font = library.fontForTypeface("Roboto-Regular");
  ASSERT_TRUE(font.isValid());

  std::string string("Oh Hai!");
  rl::type::HBBuffer buffer(font, string);

  ASSERT_EQ(buffer.length(), string.size());
}

TEST(TypographyTest, SimpleBufferIterator) {
  rl::type::FTLibrary library;
  ASSERT_TRUE(library.registerFont(rl::core::URI{"Roboto-Regular.ttf"}));

  auto font = library.fontForTypeface("Roboto-Regular");
  ASSERT_TRUE(font.isValid());

  std::string string("Oh Hai!");
  rl::type::HBBuffer buffer(font, string);

  ASSERT_EQ(buffer.length(), string.size());

  auto result =
      buffer.iterateGlyphs([](const rl::type::GlyphInfo&) { return false; });
  ASSERT_EQ(result, 1);

  auto result2 =
      buffer.iterateGlyphs([](const rl::type::GlyphInfo&) { return true; });
  ASSERT_EQ(result2, buffer.length());
}

TEST(TypographyTest, SimpleBufferIteratorCheck) {
  rl::type::FTLibrary library;
  ASSERT_TRUE(library.registerFont(rl::core::URI{"Roboto-Regular.ttf"}));

  auto font = library.fontForTypeface("Roboto-Regular");
  ASSERT_TRUE(font.isValid());

  std::string string("Ha!");
  rl::type::HBBuffer buffer(font, string);

  ASSERT_EQ(buffer.length(), string.size());

  auto result2 =
      buffer.iterateGlyphs([](const rl::type::GlyphInfo& info) -> bool {
        switch (info.cluster) {
          case 0:
            EXPECT_EQ(info.codepoint, 44);

            EXPECT_NEAR(info.advance.x, 9.98, 0.1);
            EXPECT_NEAR(info.advance.y, 0.0, 0.1);

            EXPECT_NEAR(info.offset.x, 0.0, 0.1);
            EXPECT_NEAR(info.offset.y, 0.0, 0.1);
            break;
          case 1:
            EXPECT_EQ(info.codepoint, 69);

            EXPECT_NEAR(info.advance.x, 7.61, 0.1);
            EXPECT_NEAR(info.advance.y, 0.0, 0.1);

            EXPECT_NEAR(info.offset.x, 0.0, 0.1);
            EXPECT_NEAR(info.offset.y, 0.0, 0.1);
            break;
          case 2:
            EXPECT_EQ(info.codepoint, 5);

            EXPECT_NEAR(info.advance.x, 3.61, 0.1);
            EXPECT_NEAR(info.advance.y, 0.0, 0.1);

            EXPECT_NEAR(info.offset.x, 0.0, 0.1);
            EXPECT_NEAR(info.offset.y, 0.0, 0.1);
            break;
          default:
            EXPECT_TRUE(false);
            break;
        }
        return true;
      });
  ASSERT_EQ(result2, buffer.length());
}

TEST(TypographyTest, FontSize) {
  rl::type::FTLibrary library;
  ASSERT_TRUE(library.registerFont(rl::core::URI{"Roboto-Regular.ttf"}));

  auto font = library.fontForTypeface("Roboto-Regular");
  ASSERT_TRUE(font.isValid());

  EXPECT_NEAR(font.pointSize(), 14.0, 0.2);
}

TEST(TypographyTest, FontSizeSet) {
  rl::type::FTLibrary library;
  ASSERT_TRUE(library.registerFont(rl::core::URI{"Roboto-Regular.ttf"}));

  auto font = library.fontForTypeface("Roboto-Regular");
  ASSERT_TRUE(font.isValid());

  EXPECT_NEAR(font.pointSize(), 14.0, 0.2);

  font.setPointSize(26.0);

  EXPECT_NEAR(font.pointSize(), 26.0, 0.2);
}
