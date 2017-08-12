/*
 *  This source file is part of the Radar project.
 *  Licensed under the MIT License. See LICENSE file for details.
 */

#include <TestRunner/TestRunner.h>
#include <Typography/String.h>

TEST(StringTest, SimpleString) {
  rl::type::String string("Hello");
  ASSERT_EQ(string.lengthOfCharacters(), 5);
}

TEST(StringTest, SingleEmoji) {
  rl::type::String string("😀");
  ASSERT_EQ(string.lengthOfCharacters(), 1);
  ASSERT_EQ(string.size(), 2);
}

TEST(StringTest, EmojiString) {
  rl::type::String string("😀 Hello 😀");
  ASSERT_EQ(string.lengthOfCharacters(), 9);
}

TEST(StringTest, MoreEmoji) {
  rl::type::String string(
      "😙😚😋😜😝😛🤑🤗🤓😎🤡🤠😏😒😞😔😟"
      "😕");
  ASSERT_EQ(string.lengthOfCharacters(), 18);
}

TEST(StringTest, StrignAppend) {
  rl::type::String string;
  ASSERT_EQ(string.lengthOfCharacters(), 0);
  string.append(std::string{"😀"});
  ASSERT_EQ(string.lengthOfCharacters(), 1);
  string.append(std::string{"Hello"});
  ASSERT_EQ(string.lengthOfCharacters(), 6);
  string.append(std::string{"😀"});
  ASSERT_EQ(string.lengthOfCharacters(), 7);
}
