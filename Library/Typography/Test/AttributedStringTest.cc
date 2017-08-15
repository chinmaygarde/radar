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

TEST(AttributedStringTest, DescriptorAtIndex) {
  rl::type::FontDescriptor fooDesc("Foo", 100);
  rl::type::FontDescriptor barDesc("Bar", 200);
  rl::type::AttributedStringBuilder builder;
  builder.pushFontDescriptor(fooDesc)
      .appendText("😄")
      .pushFontDescriptor(barDesc)
      .appendText("hello")
      .popFontDescriptor()
      .appendText("Again");
  auto attributedString = builder.attributedString();
  ASSERT_TRUE(attributedString.isValid());
  ASSERT_EQ(attributedString.fontDescriptorsMap().size(), 3);
  ASSERT_NE(attributedString.fontDescriptorsMap().find(0),
            attributedString.fontDescriptorsMap().end());
  ASSERT_NE(attributedString.fontDescriptorsMap().find(2),
            attributedString.fontDescriptorsMap().end());
  ASSERT_NE(attributedString.fontDescriptorsMap().find(7),
            attributedString.fontDescriptorsMap().end());
  ASSERT_DOUBLE_EQ(attributedString.descriptorForIndex(0).pointSize(),
                   100);  // smiley 0
  ASSERT_EQ(attributedString.descriptorForIndex(0).postscriptName(),
            "Foo");  // smiley 0
  ASSERT_DOUBLE_EQ(attributedString.descriptorForIndex(1).pointSize(),
                   100);  // smiley 1
  ASSERT_DOUBLE_EQ(attributedString.descriptorForIndex(2).pointSize(),
                   200);  // h
  ASSERT_EQ(attributedString.descriptorForIndex(2).postscriptName(),
            "Bar");  // h
  ASSERT_DOUBLE_EQ(attributedString.descriptorForIndex(3).pointSize(),
                   200);  // e
  ASSERT_DOUBLE_EQ(attributedString.descriptorForIndex(4).pointSize(),
                   200);  // l
  ASSERT_DOUBLE_EQ(attributedString.descriptorForIndex(5).pointSize(),
                   200);  // l
  ASSERT_DOUBLE_EQ(attributedString.descriptorForIndex(6).pointSize(),
                   200);  // o
  ASSERT_DOUBLE_EQ(attributedString.descriptorForIndex(7).pointSize(),
                   100);  // A
  ASSERT_EQ(attributedString.descriptorForIndex(7).postscriptName(),
            "Foo");  // h
  ASSERT_DOUBLE_EQ(attributedString.descriptorForIndex(8).pointSize(),
                   100);  // g
  ASSERT_DOUBLE_EQ(attributedString.descriptorForIndex(9).pointSize(),
                   100);  // a
  ASSERT_DOUBLE_EQ(attributedString.descriptorForIndex(10).pointSize(),
                   100);  // i
  ASSERT_DOUBLE_EQ(attributedString.descriptorForIndex(11).pointSize(),
                   100);  // n
  ASSERT_DOUBLE_EQ(attributedString.descriptorForIndex(12).pointSize(),
                   100);  // <past>
  ASSERT_DOUBLE_EQ(attributedString.descriptorForIndex(13).pointSize(),
                   100);  // <past>
  ASSERT_DOUBLE_EQ(attributedString.descriptorForIndex(14).pointSize(),
                   100);  // <past>
}
