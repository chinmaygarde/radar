/*
 *  This source file is part of the Radar project.
 *  Licensed under the MIT License. See LICENSE file for details.
 */

#include <Core/File.h>
#include <Core/Utilities.h>
#include <DevSupport/DevSupport.h>

namespace rl {
namespace dev {

static bool AttemptCWDUpdate(std::string directoryPath) {
  core::URI directoryURI(directoryPath);

  if (directoryURI.normalize() &&
      core::File::SetAsWorkingDirectory(directoryURI)) {
    return true;
  }

  return false;
}

bool UpdateWorkingDirectoryForFixtures() {
  auto exe = core::GetExecutablePath();

  if (!exe.isValid()) {
    return false;
  }

  auto exeString = exe.toString();

  if (AttemptCWDUpdate(exeString + "/../Fixtures")) {
    return true;
  }

  if (AttemptCWDUpdate(exeString + "/../../Fixtures")) {
    return true;
  }

  return false;
}

}  // namespace dev
}  // namespace rl
