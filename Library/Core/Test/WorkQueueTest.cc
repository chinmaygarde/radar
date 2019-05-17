/*
 *  This source file is part of the Radar project.
 *  Licensed under the MIT License. See LICENSE file for details.
 */

#include <atomic>

#include <Core/WorkQueue.h>
#include <TestRunner/TestRunner.h>

namespace rl {
namespace core {
namespace testing {

TEST(WorkQueue, SimpleInitialization) {
  WorkQueue queue;
  ASSERT_GE(queue.workerCount(), 0u);
}

TEST_SLOW(WorkQueue, SimpleWork) {
  auto size = 5000;
  std::atomic<int> count(0);

  {
    WorkQueue queue;
    ASSERT_GE(queue.workerCount(), 0u);

    for (int i = 0; i < size; i++) {
      ASSERT_TRUE(queue.dispatch([&count]() { count++; }));
    }
  }

  ASSERT_EQ(size, count.load());
}

}  // namespace testing
}  // namespace core
}  // namespace rl
