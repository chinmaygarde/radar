// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <TestRunner/TestRunner.h>

#include <Core/Mutexed.h>
#include <Core/WorkQueue.h>

#include <string>

TEST(MutexedTest, Compiles) {
  rl::core::Mutexed<std::string, std::mutex> string("Oh hai!");
  ASSERT_EQ(string.access().get(), "Oh hai!");
}

TEST(MutexedTest, DefaultsToMutex) {
  rl::core::Mutexed<std::string> string("Oh hai");
  ASSERT_EQ(string.access().get(), "Oh hai");
}

TEST_SLOW(MutexedTest, SillyCounter) {
  const size_t limit = 5;

  rl::core::Mutexed<size_t> actual(0);

  {
    rl::core::WorkQueue wq(__func__);

    for (size_t i = 0; i < limit; i++) {
      wq.dispatch([&actual]() {
        auto access = actual.access();
        auto& value = access.get();
        auto oldValue = value;
        std::this_thread::sleep_for(std::chrono::milliseconds(2));
        value = oldValue + 1;
      });
    }
  }  // So the wq can gracefully collect its workers.

  ASSERT_EQ(actual.access().get(), limit);
}
