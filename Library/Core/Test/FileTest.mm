// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <TestRunner/TestRunner.h>

#include <Core/File.h>
#include <thread>

RL_DECLARE_TEST_START(FileTest)

TEST(FileTest, SimpleFail) {
  rl::core::File file(std::string{"file://no/such/file/exists.txt"});
  ASSERT_FALSE(file.isReady());
}

RL_DECLARE_TEST_END
