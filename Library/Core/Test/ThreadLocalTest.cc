/*
 *  This source file is part of the Radar project.
 *  Licensed under the MIT License. See LICENSE file for details.
 */

#include <thread>

#include <Core/ThreadLocal.h>
#include <TestRunner/TestRunner.h>

namespace rl {
namespace core {
namespace testing {

#if RL_THREAD_LOCAL_PTHREADS

TEST(ThreadLocalTest, SimpleInitialization) {
  std::thread thread([&] {
    ThreadLocal local;
    const size_t value = 100;
    local.set(value);
    ASSERT_EQ(local.get(), value);
  });
  thread.join();
}

TEST(ThreadLocalTest, DestroyCallback) {
  std::thread thread([&] {
    size_t destroys = 0;
    ThreadLocal local([&destroys](uintptr_t) { destroys++; });

    const size_t value = 100;
    local.set(value);
    ASSERT_EQ(local.get(), value);
    ASSERT_EQ(destroys, 0u);
  });
  thread.join();
}

TEST(ThreadLocalTest, DestroyCallback2) {
  std::thread thread([&] {
    size_t destroys = 0;
    ThreadLocal local([&destroys](uintptr_t) { destroys++; });

    local.set(100);
    ASSERT_EQ(local.get(), 100u);
    ASSERT_EQ(destroys, 0u);
    local.set(200);
    ASSERT_EQ(local.get(), 200u);
    ASSERT_EQ(destroys, 1u);
  });
  thread.join();
}

TEST(ThreadLocalTest, DestroyThreadTimeline) {
  std::thread thread([&] {
    size_t destroys = 0;
    ThreadLocal local([&destroys](uintptr_t) { destroys++; });

    std::thread thread([&]() {
      local.set(100);
      ASSERT_EQ(local.get(), 100u);
      ASSERT_EQ(destroys, 0u);
      local.set(200);
      ASSERT_EQ(local.get(), 200u);
      ASSERT_EQ(destroys, 1u);
    });
    ASSERT_EQ(local.get(), 0u);
    thread.join();
    ASSERT_EQ(local.get(), 0u);
    ASSERT_EQ(destroys, 2u);
  });
  thread.join();
}

TEST(ThreadLocalTest, SettingSameValue) {
  std::thread thread([&] {
    size_t destroys = 0;
    {
      ThreadLocal local([&destroys](uintptr_t) { destroys++; });

      local.set(100);
      ASSERT_EQ(destroys, 0u);
      local.set(100);
      local.set(100);
      local.set(100);
      ASSERT_EQ(local.get(), 100u);
      local.set(100);
      local.set(100);
      ASSERT_EQ(destroys, 0u);
      local.set(200);
      ASSERT_EQ(destroys, 1u);
      ASSERT_EQ(local.get(), 200u);
    }

    ASSERT_EQ(destroys, 1u);
  });
  thread.join();
}

#endif  // RL_THREAD_LOCAL_PTHREADS

}  // namespace testing
}  // namespace core
}  // namespace rl
