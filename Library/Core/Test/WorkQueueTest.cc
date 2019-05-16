/*
 *  This source file is part of the Radar project.
 *  Licensed under the MIT License. See LICENSE file for details.
 */

#include <Core/WorkQueue.h>
#include <TestRunner/TestRunner.h>
#include <atomic>

TEST(WorkQueue, SimpleInitialization) {
  rl::core::WorkQueue queue;
  ASSERT_GE(queue.workerCount(), 0u);
}

TEST_SLOW(WorkQueue, SimpleWork) {
  auto size = 5000;
  std::atomic<int> count(0);

  {
    rl::core::WorkQueue queue;
    ASSERT_GE(queue.workerCount(), 0u);

    for (int i = 0; i < size; i++) {
      ASSERT_TRUE(queue.dispatch([&count]() { count++; }));
    }
  }

  ASSERT_EQ(size, count.load());
}
