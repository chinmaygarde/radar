// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <Core/Config.h>

#if !RL_OS_NACL

#include <Core/SharedMemory.h>
#include <gtest/gtest.h>

TEST(SharedMemoryTest, SimpleInitialization) {
  rl::SharedMemory memory(1024);

  ASSERT_TRUE(memory.isReady());
  ASSERT_TRUE(memory.size() == 1024);
}

#endif  // !RL_OS_NACL
