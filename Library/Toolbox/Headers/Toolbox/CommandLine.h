/*
 *  This source file is part of the Radar project.
 *  Licensed under the MIT License. See LICENSE file for details.
 */

#pragma once

#include <Core/Macros.h>
#include <string>
#include <unordered_map>
#include <vector>

namespace rl {
namespace toolbox {

class CommandLine {
 public:
  static CommandLine Parse(int argc, char const* argv[]);

  static CommandLine Parse(std::vector<std::string> arguments);

  CommandLine(CommandLine&&);

  ~CommandLine();

  bool hasOption(const std::string& option) const;

  bool hasListOfOptions(const std::string& option) const;

  std::vector<std::string> optionList(const std::string& option) const;

  std::vector<std::string> unnamedOptionsList() const;

  bool hasUnnamedOptionsList() const;

 private:
  using Values = std::unordered_map<std::string, std::vector<std::string>>;
  Values _values;

  CommandLine(Values values = {});

  RL_DISALLOW_COPY_AND_ASSIGN(CommandLine);
};

}  // namespace toolbox
}  // namespace rl
