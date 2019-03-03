/*
 *  This source file is part of the Radar project.
 *  Licensed under the MIT License. See LICENSE file for details.
 */

#include <TestRunner/TestRunner.h>
#include <Toolbox/CommandLine.h>

namespace rl {
namespace toolbox {

TEST(CommandLineTest, CanParseArgs) {
  auto commandLine =
      CommandLine::Parse({"--values", "1", "2", "--slow", "-another arg"});

  ASSERT_TRUE(commandLine.hasValue("values"));
  ASSERT_TRUE(commandLine.hasValue("another arg"));
  ASSERT_TRUE(commandLine.hasValue("slow"));
  ASSERT_FALSE(commandLine.hasValue("fast"));
  ASSERT_TRUE(commandLine.hasListOfValues("values"));
  ASSERT_FALSE(commandLine.hasListOfValues("slow"));
  ASSERT_FALSE(commandLine.hasListOfValues("another arg"));
  ASSERT_FALSE(commandLine.hasListOfValues("fast"));

  auto list = commandLine.listOfValues("values");
  ASSERT_EQ(list.size(), 2);
  ASSERT_EQ(list[0], "1");
  ASSERT_EQ(list[1], "2");
  ASSERT_NE(list.size(), 5);

  auto not_exist = commandLine.listOfValues("slow");
  ASSERT_EQ(not_exist.size(), 0);

  ASSERT_FALSE(commandLine.hasUnnamedListOfValues());
  ASSERT_EQ(commandLine.unnamedListOfValues().size(), 0);
}

TEST(CommandLineTest, CanParseUnnamedListOfValues) {
  auto commandLine = CommandLine::Parse(
      {"unnamed1", "unnamed2", "--values", "1", "2", "--slow", "-another arg",
       "--", "unnamed3", "unnamed4", "--something else completely", "-",
       "unnamed5"});
  ASSERT_TRUE(commandLine.hasUnnamedListOfValues());
  auto list = commandLine.unnamedListOfValues();
  ASSERT_EQ(list.size(), 5);
  ASSERT_EQ(list[0], "unnamed1");
  ASSERT_EQ(list[1], "unnamed2");
  ASSERT_EQ(list[2], "unnamed3");
  ASSERT_EQ(list[3], "unnamed4");
  ASSERT_EQ(list[4], "unnamed5");
}

}  // namespace toolbox
}  // namespace rl