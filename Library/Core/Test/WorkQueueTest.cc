// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <TestRunner/TestRunner.h>

#include <Core/Core.h>
#include <atomic>

TEST(WorkQueue, SimpleInitialization) {
  rl::core::WorkQueue queue;
  ASSERT_GE(queue.workerCount(), 0);
}

TEST_SLOW(WorkQueue, SimpleWork) {
  auto size = 5000;
  std::atomic<int> count(0);

  {
    rl::core::WorkQueue queue;
    ASSERT_GE(queue.workerCount(), 0);

    for (int i = 0; i < size; i++) {
      ASSERT_TRUE(queue.dispatch([&count]() { count++; }));
    }
  }

  ASSERT_EQ(size, count.load());
}
