// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <gtest/gtest.h>
#include <Core/ThreadLocal.h>

#if RL_THREAD_LOCAL_PTHREADS

#include <thread>

TEST(ThreadLocalTest, SimpleInitialization) {
  rl::core::ThreadLocal local;

  auto value = 100;
  local.set(value);
  ASSERT_EQ(local.get(), value);
}

TEST(ThreadLocalTest, DestroyCallback) {
  int destroys = 0;
  rl::core::ThreadLocal local([&destroys](uintptr_t) { destroys++; });

  auto value = 100;
  local.set(value);
  ASSERT_EQ(local.get(), value);
  ASSERT_EQ(destroys, 0);
}

TEST(ThreadLocalTest, DestroyCallback2) {
  int destroys = 0;
  rl::core::ThreadLocal local([&destroys](uintptr_t) { destroys++; });

  local.set(100);
  ASSERT_EQ(local.get(), 100);
  ASSERT_EQ(destroys, 0);
  local.set(200);
  ASSERT_EQ(local.get(), 200);
  ASSERT_EQ(destroys, 1);
}

TEST(ThreadLocalTest, DestroyThreadTimeline) {
  int destroys = 0;
  rl::core::ThreadLocal local([&destroys](uintptr_t) { destroys++; });

  std::thread thread([&destroys, &local]() {
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
}

TEST(ThreadLocalTest, SettingSameValue) {
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
}

#endif  // RL_THREAD_LOCAL_PTHREADS
