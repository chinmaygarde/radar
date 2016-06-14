// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <DevSupport/DevSupport.h>

#include <Core/Core.h>

namespace rl {
namespace dev {

bool UpdateWorkingDirectoryForFixtures() {
  auto exe = core::GetExecutablePath();

  if (!exe.isValid()) {
    return false;
  }

  /*
   *  Look for fixtures in the executable folder.
   */

  core::URI fixturesInExeDir(exe.append(core::URI{"../Fixtures"}));

  if (core::File::SetAsWorkingDirectory(fixturesInExeDir)) {
    return true;
  }

  /*
   *  Look for fixtures one level above the executable directory.
   */

  core::URI fixturesInContainingDir(exe.append(core::URI{"../../Fixtures"}));
  if (core::File::SetAsWorkingDirectory(fixturesInContainingDir)) {
    return true;
  }

  return false;
}

}  // namespace dev
}  // namespace rl
