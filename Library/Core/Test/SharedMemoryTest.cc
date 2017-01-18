// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "Config.h"

#if RL_SHMEM != RL_SHMEM_DISABLED

#include <Core/SharedMemory.h>
#include <TestRunner/TestRunner.h>

TEST(SharedMemoryTest, SimpleInitialization) {
  rl::core::SharedMemory memory(1024);

  ASSERT_TRUE(memory.isReady());
  ASSERT_TRUE(memory.size() == 1024);
}

#endif  // RL_SHMEM != RL_SHMEM_DISABLED
