/*
 *  This source file is part of the Radar project.
 *  Licensed under the MIT License. See LICENSE file for details.
 */

#include <TestRunner/TestRunner.h>
#include <Typography/String.h>

TEST(StringTest, SimpleString) {
  rl::type::String string("Hello");
  ASSERT_EQ(string.lengthOfCharacters(), 5u);
}

TEST(StringTest, SingleEmoji) {
  rl::type::String string("😀");
  ASSERT_EQ(string.lengthOfCharacters(), 1u);
  ASSERT_EQ(string.size(), 2u);
}

TEST(StringTest, EmojiString) {
  rl::type::String string("😀 Hello 😀");
  ASSERT_EQ(string.lengthOfCharacters(), 9u);
}

TEST(StringTest, MoreEmoji) {
  rl::type::String string(
      "😙😚😋😜😝😛🤑🤗🤓😎🤡🤠😏😒😞😔😟"
      "😕");
  ASSERT_EQ(string.lengthOfCharacters(), 18u);
}

TEST(StringTest, StringAppend) {
  rl::type::String string;
  ASSERT_EQ(string.lengthOfCharacters(), 0u);
  string.append(std::string{"😀"});
  ASSERT_EQ(string.lengthOfCharacters(), 1u);
  string.append(std::string{"Hello"});
  ASSERT_EQ(string.lengthOfCharacters(), 6u);
  string.append(std::string{"😀"});
  ASSERT_EQ(string.lengthOfCharacters(), 7u);
}
