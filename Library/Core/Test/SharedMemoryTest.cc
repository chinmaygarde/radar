/*
 *  This source file is part of the Radar project.
 *  Licensed under the MIT License. See LICENSE file for details.
 */

#include "Config.h"

#if RL_SHMEM != RL_SHMEM_DISABLED

#include <Core/SharedMemory.h>
#include <TestRunner/TestRunner.h>

TEST(SharedMemoryTest, SimpleInitialization) {
  rl::core::SharedMemory memory(1024);

  ASSERT_TRUE(memory.isReady());
  ASSERT_EQ(memory.size(), 1024u);
}

#endif  // RL_SHMEM != RL_SHMEM_DISABLED
