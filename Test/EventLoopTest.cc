// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <stdio.h>

#include <Core/Core.h>
#include <gtest/gtest.h>
#include <thread>

TEST(EventLoopTest, CurrentEventLoopAccess) {
  rl::EventLoop* loop = rl::EventLoop::Current();
  ASSERT_TRUE(loop != nullptr);
  ASSERT_TRUE(rl::EventLoop::Current() == loop);
}

TEST(EventLoopTest, EventLoopOnAnotherThread) {
  rl::EventLoop* loop1 = rl::EventLoop::Current();
  rl::EventLoop* loop2 = nullptr;

  std::thread thread([&] { loop2 = rl::EventLoop::Current(); });

  thread.join();

  ASSERT_TRUE(loop1 != loop2);
}

TEST(EventLoopTest, SimpleLoop) {
  std::thread thread([] {
    auto outer = rl::EventLoop::Current();

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

TEST(EventLoopTest, Timer) {
  std::thread timerThread([] {

    auto loop = rl::EventLoop::Current();

    std::chrono::high_resolution_clock clock;

    auto start = clock.now();

    auto timer = rl::EventLoopSource::Timer(std::chrono::milliseconds(10));

    timer->setWakeFunction([clock, start]() {
      long long duration =
          std::chrono::duration_cast<std::chrono::milliseconds>(clock.now() -
                                                                start)
              .count();
      rl::EventLoop::Current()->terminate();
      ASSERT_TRUE(duration >= 5 &&
                  duration <=
                      15); /* FIXME: brittle, need a better way to check. */
    });

    loop->addSource(timer);

    loop->loop();

  });

  timerThread.join();
}

TEST(EventLoopTest, TimerRepetition) {
  int count = 0;

  std::thread timerThread([&count] {

    auto loop = rl::EventLoop::Current();

    auto timer = rl::EventLoopSource::Timer(std::chrono::milliseconds(1));

    timer->setWakeFunction([&count, loop]() {

      count++;

      if (count == 10) {
        loop->terminate();
      }
    });

    loop->addSource(timer);

    loop->loop();

  });

  timerThread.join();

  ASSERT_TRUE(count == 10);
}
