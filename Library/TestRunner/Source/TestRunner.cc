// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <DevSupport/DevSupport.h>
#include <gtest/gtest.h>

int main(int argc, char** argv) {
  rl::dev::UpdateWorkingDirectoryForFixtures();

  ::testing::InitGoogleTest(&argc, argv);

  return RUN_ALL_TESTS();
}
