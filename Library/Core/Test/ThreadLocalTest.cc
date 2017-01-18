// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <Core/ThreadLocal.h>
#include <TestRunner/TestRunner.h>

#if RL_THREAD_LOCAL_PTHREADS

#include <thread>

TEST(ThreadLocalTest, SimpleInitialization) {
  std::thread thread([&] {
    rl::core::ThreadLocal local;
    auto value = 100;
    local.set(value);
    ASSERT_EQ(local.get(), value);
  });
  thread.join();
}

TEST(ThreadLocalTest, DestroyCallback) {
  std::thread thread([&] {
    int destroys = 0;
    rl::core::ThreadLocal local([&destroys](uintptr_t) { destroys++; });

    auto value = 100;
    local.set(value);
    ASSERT_EQ(local.get(), value);
    ASSERT_EQ(destroys, 0);
  });
  thread.join();
}

TEST(ThreadLocalTest, DestroyCallback2) {
  std::thread thread([&] {
    int destroys = 0;
    rl::core::ThreadLocal local([&destroys](uintptr_t) { destroys++; });

    local.set(100);
    ASSERT_EQ(local.get(), 100);
    ASSERT_EQ(destroys, 0);
    local.set(200);
    ASSERT_EQ(local.get(), 200);
    ASSERT_EQ(destroys, 1);
  });
  thread.join();
}

TEST(ThreadLocalTest, DestroyThreadTimeline) {
  std::thread thread([&] {
    int destroys = 0;
    rl::core::ThreadLocal local([&destroys](uintptr_t) { destroys++; });

    std::thread thread([&]() {
      local.set(100);
      ASSERT_EQ(local.get(), 100);
      ASSERT_EQ(destroys, 0);
      local.set(200);
      ASSERT_EQ(local.get(), 200);
      ASSERT_EQ(destroys, 1);
    });
    ASSERT_EQ(local.get(), 0);
    thread.join();
    ASSERT_EQ(local.get(), 0);
    ASSERT_EQ(destroys, 2);
  });
  thread.join();
}

TEST(ThreadLocalTest, SettingSameValue) {
  std::thread thread([&] {
    int destroys = 0;
    {
      rl::core::ThreadLocal local([&destroys](uintptr_t) { destroys++; });

      local.set(100);
      ASSERT_EQ(destroys, 0);
      local.set(100);
      local.set(100);
      local.set(100);
      ASSERT_EQ(local.get(), 100);
      local.set(100);
      local.set(100);
      ASSERT_EQ(destroys, 0);
      local.set(200);
      ASSERT_EQ(destroys, 1);
      ASSERT_EQ(local.get(), 200);
    }

    ASSERT_EQ(destroys, 1);
  });
  thread.join();
}

#endif  // RL_THREAD_LOCAL_PTHREADS
