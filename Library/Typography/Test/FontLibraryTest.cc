/*
 *  This source file is part of the Radar project.
 *  Licensed under the MIT License. See LICENSE file for details.
 */

#include <TestRunner/TestRunner.h>
#include <Typography/FontLibrary.h>

TEST(FontLibraryTest, SimpleFontRegistration) {
  rl::type::FontLibrary library;
  ASSERT_TRUE(library.registerFont(rl::core::URI{"Roboto-Regular.ttf"}, 0));
  ASSERT_EQ(library.registeredFonts(), 1u);
}

TEST(FontLibraryTest, CannotRegisterSameFontAgain) {
  rl::type::FontLibrary library;
  ASSERT_TRUE(library.registerFont(rl::core::URI{"Roboto-Regular.ttf"}, 0));
  ASSERT_EQ(library.registeredFonts(), 1u);
  ASSERT_FALSE(library.registerFont(rl::core::URI{"Roboto-Regular.ttf"}, 0));
  ASSERT_EQ(library.registeredFonts(), 1u);
}

TEST(FontLibraryTest, FindByName) {
  rl::type::FontLibrary library;
  ASSERT_TRUE(library.registerFont(rl::core::URI{"Roboto-Regular.ttf"}, 0));
  ASSERT_EQ(library.registeredFonts(), 1u);
  auto font = library.fontForDescriptor({"Roboto-Regular", 14.0});
  ASSERT_TRUE(font.isValid());
  ASSERT_EQ(font.postscriptName(), "Roboto-Regular");
  ASSERT_DOUBLE_EQ(font.size(), 14.0);
}

TEST(FontLibraryTest, FaceHasGlyphs) {
  rl::type::FontLibrary library;
  ASSERT_TRUE(library.registerFont(rl::core::URI{"Roboto-Regular.ttf"}, 0));
  ASSERT_EQ(library.registeredFonts(), 1u);
  auto face = library.faceForDescriptor({"Roboto-Regular", 14.0});
  ASSERT_NE(face, nullptr);
  ASSERT_EQ(face->glyphCount(), 1250u);
}
