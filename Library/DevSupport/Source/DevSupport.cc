// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

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
