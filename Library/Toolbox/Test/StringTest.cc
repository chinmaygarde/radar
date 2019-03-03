/*
 *  This source file is part of the Radar project.
 *  Licensed under the MIT License. See LICENSE file for details.
 */

#include <TestRunner/TestRunner.h>
#include <Toolbox/String.h>

namespace rl {
namespace toolbox {
namespace string {

TEST(StringTest, ContainsTest) {
  auto string = std::string{"some string here"};
  ASSERT_TRUE(Contains(string, std::string{"string"}));
  ASSERT_TRUE(Contains(string, std::string{"e string h"}));
  ASSERT_FALSE(Contains(string, std::string{"nomatch"}));
  ASSERT_FALSE(Contains(string, std::string{""}));
}

TEST(StringTest, PrefixTest) {
  auto string = std::string{"some string here"};
  ASSERT_TRUE(HasPrefix(string, std::string{"some"}));
  ASSERT_TRUE(HasPrefix(string, std::string{"som"}));
  ASSERT_TRUE(HasPrefix(string, std::string{"some string here"}));
  ASSERT_FALSE(HasPrefix(string, std::string{"something here"}));
  ASSERT_FALSE(HasPrefix(string, std::string{"something"}));
  ASSERT_FALSE(HasPrefix(string, std::string{""}));
}

TEST(StringTest, SuffixTest) {
  auto string = std::string{"some string here"};
  ASSERT_TRUE(HasSuffix(string, std::string{"here"}));
  ASSERT_TRUE(HasSuffix(string, std::string{"string here"}));
  ASSERT_TRUE(HasSuffix(string, std::string{" string here"}));
  ASSERT_TRUE(HasSuffix(string, std::string{"some string here"}));
  ASSERT_FALSE(HasSuffix(string, std::string{"1 some string here"}));
  ASSERT_FALSE(HasSuffix(string, std::string{"11 some string here"}));
  ASSERT_FALSE(HasSuffix(string, std::string{"nothing here"}));
  ASSERT_FALSE(HasSuffix(string, std::string{"~@#$"}));
  ASSERT_FALSE(HasSuffix(string, std::string{""}));
}

}  // namespace string
}  // namespace toolbox
}  // namespace rl
