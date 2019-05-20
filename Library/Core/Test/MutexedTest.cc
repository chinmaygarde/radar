/*
 *  This source file is part of the Radar project.
 *  Licensed under the MIT License. See LICENSE file for details.
 */

#include <string>

#include <Core/Mutexed.h>
#include <Core/WorkQueue.h>
#include <TestRunner/TestRunner.h>

namespace rl {
namespace core {
namespace testing {

TEST(MutexedTest, Compiles) {
  Mutexed<std::string, Mutex> string("Oh hai!");
  ASSERT_EQ(string.access().get(), "Oh hai!");
}

TEST(MutexedTest, DefaultsToMutex) {
  Mutexed<std::string> string("Oh hai");
  ASSERT_EQ(string.access().get(), "Oh hai");
}

TEST_SLOW(MutexedTest, SillyCounter) {
#if !RL_OS_MAC
  GTEST_SKIP();
#endif  //  !RL_OS_MAC

  const size_t limit = 5;

  Mutexed<size_t> actual(0);

  {
    WorkQueue wq;

    for (size_t i = 0; i < limit; i++) {
      auto dispatched = wq.dispatch([&actual]() {
        auto access = actual.access();
        auto& value = access.get();
        auto oldValue = value;
        std::this_thread::sleep_for(std::chrono::milliseconds(2));
        value = oldValue + 1;
      });
      ASSERT_TRUE(dispatched);
    }
  }  // So the wq can gracefully collect its workers.

  ASSERT_EQ(actual.access().get(), limit);
}

}  // namespace testing
}  // namespace core
}  // namespace rl
