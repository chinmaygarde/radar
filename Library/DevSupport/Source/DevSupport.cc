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

  core::File fixturesInExeDir(exe.append(core::URI{"../Fixtures"}));

  if (fixturesInExeDir.setAsWorkingDirectory()) {
    return true;
  }

  /*
   *  Look for fixtures one level above the executable directory.
   */

  core::File fixturesInContainingDir(exe.append(core::URI{"../../Fixtures"}));
  if (fixturesInContainingDir.setAsWorkingDirectory()) {
    return true;
  }

  return false;
}

}  // namespace dev
}  // namespace rl
