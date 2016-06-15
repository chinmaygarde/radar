// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <TestRunner/TestRunner.h>

#include <Core/Core.h>

#include <thread>

#if !RL_OS_BSD && !RL_OS_WINDOWS && !RL_OS_NACL

TEST(ThreadTest, GetSetName) {
  std::thread thread([&] {
    const char name[] = "DeRp";
    rl::core::thread::SetName(name);
    ASSERT_EQ(
        ::strncmp(name, rl::core::thread::GetName().c_str(), sizeof(name)), 0);
  });
  thread.join();
}

#endif  // !RL_OS_BSD && !RL_OS_WINDOWS && !RL_OS_NACL
