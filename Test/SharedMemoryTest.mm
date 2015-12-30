// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <Core/Config.h>

#if RL_SHMEM != RL_SHMEM_DISABLED

#include "RadarTest.h"

#include <Core/SharedMemory.h>

RL_DECLARE_TEST_START(SharedMemoryTest)

TEST(SharedMemoryTest, SimpleInitialization) {
  rl::core::SharedMemory memory(1024);

  ASSERT_TRUE(memory.isReady());
  ASSERT_TRUE(memory.size() == 1024);
}

RL_DECLARE_TEST_END

#endif  // RL_SHMEM != RL_SHMEM_DISABLED
