/*
 *  This source file is part of the Radar project.
 *  Licensed under the MIT License. See LICENSE file for details.
 */

#include "Config.h"

#include <Core/SharedMemory.h>
#include <TestRunner/TestRunner.h>

namespace rl {
namespace core {
namespace testing {

#if RL_SHMEM != RL_SHMEM_DISABLED

TEST(SharedMemoryTest, SimpleInitialization) {
  rl::core::SharedMemory memory(1024);

  ASSERT_TRUE(memory.isReady());
  ASSERT_EQ(memory.size(), 1024u);
}

#endif  // RL_SHMEM != RL_SHMEM_DISABLED

}  // namespace testing
}  // namespace core
}  // namespace rl
