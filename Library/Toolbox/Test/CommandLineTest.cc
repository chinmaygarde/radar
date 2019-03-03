/*
 *  This source file is part of the Radar project.
 *  Licensed under the MIT License. See LICENSE file for details.
 */

#include <TestRunner/TestRunner.h>
#include <Toolbox/CommandLine.h>

namespace rl {
namespace toolbox {

TEST(CommandLineTest, CanParseArgs) {
  auto command_line =
      CommandLine::Parse({"--values", "1", "2", "--slow", "-another arg"});

  ASSERT_TRUE(command_line.hasOption("values"));
  ASSERT_TRUE(command_line.hasOption("another arg"));
  ASSERT_TRUE(command_line.hasOption("slow"));
  ASSERT_FALSE(command_line.hasOption("fast"));
  ASSERT_TRUE(command_line.hasListOfOptions("values"));
  ASSERT_FALSE(command_line.hasListOfOptions("slow"));
  ASSERT_FALSE(command_line.hasListOfOptions("another arg"));
  ASSERT_FALSE(command_line.hasListOfOptions("fast"));

  auto list = command_line.optionList("values");
  ASSERT_EQ(list.size(), 2);
  ASSERT_EQ(list[0], "1");
  ASSERT_EQ(list[1], "2");
  ASSERT_NE(list.size(), 5);

  auto not_exist = command_line.optionList("slow");
  ASSERT_EQ(not_exist.size(), 0);

  ASSERT_FALSE(command_line.hasUnnamedOptionsList());
  ASSERT_EQ(command_line.unnamedOptionsList().size(), 0);
}

TEST(CommandLineTest, CanParseUnnamedOptionsList) {
  auto command_line = CommandLine::Parse(
      {"unnamed1", "unnamed2", "--values", "1", "2", "--slow", "-another arg",
       "--", "unnamed3", "unnamed4", "--something else completely", "-",
       "unnamed5"});
  ASSERT_TRUE(command_line.hasUnnamedOptionsList());
  auto list = command_line.unnamedOptionsList();
  ASSERT_EQ(list.size(), 5);
  ASSERT_EQ(list[0], "unnamed1");
  ASSERT_EQ(list[1], "unnamed2");
  ASSERT_EQ(list[2], "unnamed3");
  ASSERT_EQ(list[3], "unnamed4");
  ASSERT_EQ(list[4], "unnamed5");
}

}  // namespace toolbox
}  // namespace rl