/*
 *  This source file is part of the Radar project.
 *  Licensed under the MIT License. See LICENSE file for details.
 */

#include <Core/FileHandle.h>
#include <Core/FileMapping.h>
#include <Toolbox/CommandLine.h>

const char* kMasterFileFlag = "master";

int main(int argc, char const* argv[]) {
  auto commandLine = rl::toolbox::CommandLine::Parse(argc, argv);

  if (!commandLine.hasValue(kMasterFileFlag)) {
    RL_LOG("Master dictionary from https://haveibeenpwned.com/ not specified.");
    return EXIT_FAILURE;
  }

  const auto value = commandLine.value(kMasterFileFlag);

  rl::core::FileHandle masterFileHandle(rl::core::URI{value});

  if (!masterFileHandle.isValid()) {
    RL_LOG("Could not open master file at %s", value.c_str());
    return EXIT_FAILURE;
  }

  rl::core::FileMapping masterMapping(masterFileHandle);

  if (masterMapping.mapping() == nullptr) {
    RL_LOG("Could not open the master file mapping.");
    return EXIT_FAILURE;
  }

  RL_LOG("Master file mapping size was %zu", masterMapping.size());

  return EXIT_SUCCESS;
}
