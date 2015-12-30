// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "RadarTest.h"

#include <Core/Core.h>
#include <atomic>

RL_DECLARE_TEST_START(WorkQueueTest)

TEST(WorkQueue, SimpleInitialization) {
  rl::core::WorkQueue queue("test");
  ASSERT_GE(queue.workerCount(), 0);
}

TEST(WorkQueue, SimpleWork) {
  auto size = 5000;
  std::atomic<int> count(0);

  {
    rl::core::WorkQueue queue("simple");
    ASSERT_GE(queue.workerCount(), 0);

    for (int i = 0; i < size; i++) {
      queue.dispatch([&count]() { count++; });
    }
  }

  ASSERT_EQ(size, count.load());
}

RL_DECLARE_TEST_END
