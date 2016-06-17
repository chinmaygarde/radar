// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <DevSupport/DevSupport.h>

#include <Core/Core.h>

namespace rl {
namespace dev {

static bool AttemptCWDUpdate(std::string directoryPath) {
  core::URI directoryURI(directoryPath);

  if (directoryURI.normalize() &&
      core::File::SetAsWorkingDirectory(directoryURI)) {
    RL_LOG("Current working directory updated to: '%s'",
           directoryURI.filesystemRepresentation().c_str());
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

  /*
   *  Look for fixtures in the executable folder.
   */

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
