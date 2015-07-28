// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <stdio.h>

#include "Looper.h"
#include <gtest/gtest.h>
#include <thread>

TEST(LooperTest, CurrentLooperAccess) {
  rl::Looper* looper = rl::Looper::Current();
  ASSERT_TRUE(looper != nullptr);
  ASSERT_TRUE(rl::Looper::Current() == looper);
}

TEST(LooperTest, LooperOnAnotherThread) {
  rl::Looper* looper1 = rl::Looper::Current();
  rl::Looper* looper2 = nullptr;

  std::thread thread([&] { looper2 = rl::Looper::Current(); });

  thread.join();

  ASSERT_TRUE(looper1 != looper2);
}

TEST(LooperTest, SimpleLoop) {
  std::thread thread([] {
    auto outer = rl::Looper::Current();

    bool terminatedFromInner = false;

    std::thread innerThread([&] {
      std::this_thread::sleep_for(std::chrono::milliseconds(10));
      terminatedFromInner = true;
      outer->terminate();
      ASSERT_TRUE(true);
    });

    /*
     *  Keep looping till the inner thread terminates the loop.
     */
    outer->loop();

    ASSERT_TRUE(terminatedFromInner);

    innerThread.join();
  });

  thread.join();
  ASSERT_TRUE(true);
}

TEST(LooperTest, Timer) {
  std::thread timerThread([] {

    auto looper = rl::Looper::Current();

    std::chrono::high_resolution_clock clock;

    auto start = clock.now();

    auto timer = rl::LooperSource::AsTimer(std::chrono::milliseconds(10));

    timer->setWakeFunction([clock, start]() {
      long long duration =
          std::chrono::duration_cast<std::chrono::milliseconds>(clock.now() -
                                                                start)
              .count();
      rl::Looper::Current()->terminate();
      ASSERT_TRUE(duration >= 5 &&
                  duration <=
                      15); /* FIXME: brittle, need a better way to check. */
    });

    looper->addSource(timer);

    looper->loop();

  });

  timerThread.join();
}

TEST(LooperTest, TimerRepetition) {
  int count = 0;

  std::thread timerThread([&count] {

    auto looper = rl::Looper::Current();

    auto timer = rl::LooperSource::AsTimer(std::chrono::milliseconds(1));

    timer->setWakeFunction([&count, looper]() {

      count++;

      if (count == 10) {
        looper->terminate();
      }
    });

    looper->addSource(timer);

    looper->loop();

  });

  timerThread.join();

  ASSERT_TRUE(count == 10);
}
