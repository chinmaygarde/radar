/*
 *  This source file is part of the Radar project.
 *  Licensed under the MIT License. See LICENSE file for details.
 */

#include <Toolbox/CommandLine.h>
#include <Toolbox/String.h>

namespace rl {
namespace toolbox {

CommandLine::CommandLine(CommandLine::Values values)
    : _values(std::move(values)) {}

CommandLine::~CommandLine() = default;

CommandLine::CommandLine(CommandLine&&) = default;

CommandLine CommandLine::Parse(int argc, char const* argv[]) {
  if (argc <= 0 || argv == nullptr) {
    return {};
  }

  std::vector<std::string> rawArguments;

  for (int i = 0; i < argc; ++i) {
    if (argv[i] == nullptr) {
      continue;
    }
    rawArguments.emplace_back(std::string{argv[i]});
  }

  return Parse(std::move(rawArguments));
}

static std::string RemoveCommonKeyPrefixes(std::string argument) {
  if (string::HasPrefix(argument, "--")) {
    return argument.substr(2);
  }

  if (string::HasPrefix(argument, "-")) {
    return argument.substr(1);
  }

  return argument;
}

CommandLine CommandLine::Parse(std::vector<std::string> arguments) {
  CommandLine::Values values;
  std::string lastKey;
  for (const auto& argument : arguments) {
    if (argument.size() == 0) {
      continue;
    }

    bool isKey =
        string::HasPrefix(argument, "--") || string::HasPrefix(argument, "-");

    if (isKey) {
      lastKey = RemoveCommonKeyPrefixes(argument);
    }

    if (values.count(lastKey) == 0) {
      values[lastKey] = {};
    }

    if (!isKey) {
      values[lastKey].push_back(argument);
    }
  }

  return CommandLine(std::move(values));
}

bool CommandLine::hasValue(const std::string& option) const {
  if (option.size() == 0) {
    // The zero length string is reserved for unnamed list of arguments.
    return false;
  }
  return _values.count(option) > 0;
}

bool CommandLine::hasListOfValues(const std::string& option) const {
  if (!hasValue(option)) {
    return false;
  }

  return _values.at(option).size() > 1;
}

std::vector<std::string> CommandLine::listOfValues(
    const std::string& option) const {
  if (!hasListOfValues(option)) {
    return {};
  }

  return _values.at(option);
}

std::vector<std::string> CommandLine::unnamedListOfValues() const {
  if (!hasUnnamedListOfValues()) {
    return {};
  }

  return _values.at("");
}

bool CommandLine::hasUnnamedListOfValues() const {
  return _values.count("") != 0;
}

std::string CommandLine::value(const std::string& option) const {
  if (!hasValue(option)) {
    return "";
  }

  const auto& values = _values.at(option);

  if (values.size() != 1) {
    return "";
  }

  return values[0];
}

}  // namespace toolbox
}  // namespace rl
