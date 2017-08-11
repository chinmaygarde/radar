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
