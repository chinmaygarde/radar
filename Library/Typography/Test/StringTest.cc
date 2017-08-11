/*
 *  This source file is part of the Radar project.
 *  Licensed under the MIT License. See LICENSE file for details.
 */

#include <TestRunner/TestRunner.h>
#include <Typography/String.h>

TEST(StringTest, SimpleString) {
  rl::type::String string("Hello");
  ASSERT_EQ(string.length(), 5);
}

TEST(StringTest, SingleEmoji) {
  rl::type::String string("😀");
  ASSERT_EQ(string.length(), 1);
}

TEST(StringTest, EmojiString) {
  rl::type::String string("😀 Hello 😀");
  ASSERT_EQ(string.length(), 9);
}

TEST(StringTest, MoreEmoji) {
  rl::type::String string(
      "😙😚😋😜😝😛🤑🤗🤓😎🤡🤠😏😒😞😔😟"
      "😕");
  ASSERT_EQ(string.length(), 18);
}

TEST(StringTest, StrignAppend) {
  rl::type::String string;
  ASSERT_EQ(string.length(), 0);
  string.append(std::string{"😀"});
  ASSERT_EQ(string.length(), 1);
  string.append(std::string{"Hello"});
  ASSERT_EQ(string.length(), 6);
  string.append(std::string{"😀"});
  ASSERT_EQ(string.length(), 7);
}
