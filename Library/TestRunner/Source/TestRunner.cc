// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <Core/Core.h>

#include <gtest/gtest.h>

bool UpdateWorkingDirectory() {
  auto exe = rl::core::GetExecutablePath();

  if (!exe.isValid()) {
    return false;
  }

  /*
   *  Look for fixtures in the executable folder.
   */

  rl::core::File fixturesInExeDir(exe.append(rl::core::URI{"../Fixtures"}));

  if (fixturesInExeDir.setAsWorkingDirectory()) {
    return true;
  }

  /*
   *  Look for fixtures one level above the executable directory.
   */

  rl::core::File fixturesInContainingDir(
      exe.append(rl::core::URI{"../../Fixtures"}));
  if (fixturesInContainingDir.setAsWorkingDirectory()) {
    return true;
  }

  return false;
}

int main(int argc, char** argv) {
  UpdateWorkingDirectory();

  ::testing::InitGoogleTest(&argc, argv);

  return RUN_ALL_TESTS();
}
