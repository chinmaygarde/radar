// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <TestRunner/TestRunner.h>
#include <HBBuffer.h>
#include <FTLibrary.h>

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