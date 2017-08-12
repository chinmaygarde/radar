/*
 *  This source file is part of the Radar project.
 *  Licensed under the MIT License. See LICENSE file for details.
 */

#include <TestRunner/TestRunner.h>
#include <Typography/AttributedStringBuilder.h>

TEST(AttributedStringTest, SimpleString) {
  rl::type::AttributedStringBuilder builder;
  std::string hello("Hello");
  builder.appendText(hello);
  auto attributedString = builder.attributedString();
  ASSERT_TRUE(attributedString.isValid());
  ASSERT_EQ(attributedString.fontDescriptorsMap().size(), 1);
  ASSERT_EQ(attributedString.string().lengthOfCharacters(), hello.size());
}

TEST(AttributedStringTest, SizeInDescriptorsMapIsBufferSize) {
  rl::type::FontDescriptor desc("Foo", 12);
  rl::type::AttributedStringBuilder builder;
  builder.pushFontDescriptor(desc)
      .appendText("😄")
      .popFontDescriptor()
      .appendText("World");
  auto attributedString = builder.attributedString();
  ASSERT_TRUE(attributedString.isValid());
  ASSERT_EQ(attributedString.fontDescriptorsMap().size(), 2);
  /*
   *  The first character is an emoji that takes two bytes.
   */
  ASSERT_NE(attributedString.fontDescriptorsMap().find(2),
            attributedString.fontDescriptorsMap().end());
}

TEST(AttributedStringTest, PushDescriptor) {
  rl::type::AttributedStringBuilder builder;
  std::string hello("Hello");
  rl::type::FontDescriptor desc("Foo", 12);
  builder.pushFontDescriptor(desc).appendText(hello);
  auto attributedString = builder.attributedString();
  ASSERT_TRUE(attributedString.isValid());
  ASSERT_EQ(attributedString.fontDescriptorsMap().size(), 1);
  ASSERT_EQ(attributedString.string().lengthOfCharacters(), hello.size());
}

TEST(AttributedStringTest, PushDescriptorEnd) {
  rl::type::AttributedStringBuilder builder;
  std::string hello("Hello");
  rl::type::FontDescriptor desc("Foo", 12);
  builder.appendText(hello).pushFontDescriptor(desc);
  auto attributedString = builder.attributedString();
  ASSERT_TRUE(attributedString.isValid());
  ASSERT_EQ(attributedString.fontDescriptorsMap().size(), 1);
  ASSERT_EQ(attributedString.string().lengthOfCharacters(), hello.size());
}

TEST(AttributedStringTest, PushDescriptorSame) {
  rl::type::AttributedStringBuilder builder;
  std::string hello("Hello");
  rl::type::FontDescriptor desc("Foo", 12);
  builder.pushFontDescriptor(desc)
      .appendText(hello)
      .pushFontDescriptor(desc)
      .appendText(hello);
  auto attributedString = builder.attributedString();
  ASSERT_TRUE(attributedString.isValid());
  ASSERT_EQ(attributedString.fontDescriptorsMap().size(), 1);
  ASSERT_EQ(attributedString.string().lengthOfCharacters(), hello.size() * 2);
}

TEST(AttributedStringTest, PopDescriptor) {
  rl::type::AttributedStringBuilder builder;
  std::string hello("Hello");
  rl::type::FontDescriptor desc("Foo", 12);
  builder.pushFontDescriptor(desc)
      .appendText(hello)
      .popFontDescriptor()
      .appendText(hello);
  auto attributedString = builder.attributedString();
  ASSERT_TRUE(attributedString.isValid());
  ASSERT_EQ(attributedString.fontDescriptorsMap().size(), 2);
  ASSERT_EQ(attributedString.string().lengthOfCharacters(), hello.size() * 2);
}

TEST(AttributedStringTest, TooManyPopDescriptor) {
  rl::type::AttributedStringBuilder builder;
  std::string hello("Hello");
  rl::type::FontDescriptor desc("Foo", 12);
  builder.pushFontDescriptor(desc)
      .appendText(hello)
      .popFontDescriptor()
      .popFontDescriptor()
      .appendText(hello);
  auto attributedString = builder.attributedString();
  ASSERT_TRUE(attributedString.isValid());
  ASSERT_EQ(attributedString.fontDescriptorsMap().size(), 2);
  ASSERT_EQ(attributedString.string().lengthOfCharacters(), hello.size() * 2);
}

TEST(AttributedStringTest, TooManyPushDescriptor) {
  rl::type::AttributedStringBuilder builder;
  std::string hello("Hello");
  rl::type::FontDescriptor desc("Foo", 12);
  builder.pushFontDescriptor(desc)
      .pushFontDescriptor(desc)
      .pushFontDescriptor(desc)
      .pushFontDescriptor(desc)
      .appendText(hello)
      .popFontDescriptor()
      .popFontDescriptor()
      .appendText(hello);
  auto attributedString = builder.attributedString();
  ASSERT_TRUE(attributedString.isValid());
  ASSERT_EQ(attributedString.fontDescriptorsMap().size(), 2);
  ASSERT_EQ(attributedString.string().lengthOfCharacters(), hello.size() * 2);
}
