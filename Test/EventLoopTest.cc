// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <stdio.h>

#include <Core/Core.h>
#include <gtest/gtest.h>
#include <thread>

TEST(EventLoopTest, CurrentEventLoopAccess) {
  auto loop = rl::core::EventLoop::Current();
  ASSERT_TRUE(loop != nullptr);
  ASSERT_TRUE(rl::core::EventLoop::Current() == loop);
}

TEST(EventLoopTest, EventLoopOnAnotherThread) {
  rl::core::EventLoop* loop1 = rl::core::EventLoop::Current();
  rl::core::EventLoop* loop2 = nullptr;

  std::thread thread([&] { loop2 = rl::core::EventLoop::Current(); });

  thread.join();

  ASSERT_TRUE(loop1 != loop2);
}

TEST(EventLoopTest, SimpleLoop) {
  std::thread thread([] {
    auto outer = rl::core::EventLoop::Current();

    bool terminatedFromInner = false;

    std::thread innerThread([&] {
      std::this_thread::sleep_for(rl::core::ClockDurationMilli(10));
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

    auto loop = rl::core::EventLoop::Current();

    rl::core::Clock clock;

    auto start = clock.now();

    auto timer =
        rl::core::EventLoopSource::Timer(rl::core::ClockDurationMilli(10));

    /*
     *  This test is extremely brittle :/
     */
    timer->setWakeFunction([clock, start]() {
      auto duration = std::chrono::duration_cast<rl::core::ClockDurationMilli>(
          clock.now() - start);
      rl::core::EventLoop::Current()->terminate();
      ASSERT_TRUE(duration.count() >= 5 && duration.count() <= 15);
    });

    loop->addSource(timer);

    loop->loop();

  });

  timerThread.join();
}

TEST(EventLoopTest, TimerRepetition) {
  int count = 0;

  std::thread timerThread([&count] {

    auto loop = rl::core::EventLoop::Current();

    auto timer =
        rl::core::EventLoopSource::Timer(rl::core::ClockDurationMilli(1));

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

TEST(EventLoopTest, TrivialTriggerFiresOnces) {
  int count = 0;
  std::thread trivialThread([&count] {
    auto loop = rl::core::EventLoop::Current();
    auto trivial = rl::core::EventLoopSource::Trivial();
    trivial->setWakeFunction([&count]() { count++; });

    /**
     *  Fire the trivial source multiple times. We need to assert that the
     *  callback is only triggered once. Admittedly, this is a bit tricky to
     *  test in a non brittle manner.
     */
    trivial->writer()(trivial->writeHandle());
    trivial->writer()(trivial->writeHandle());
    trivial->writer()(trivial->writeHandle());
    trivial->writer()(trivial->writeHandle());
    trivial->writer()(trivial->writeHandle());

    auto timer =
        rl::core::EventLoopSource::Timer(rl::core::ClockDurationMilli(100));

    timer->setWakeFunction(
        []() { rl::core::EventLoop::Current()->terminate(); });

    loop->addSource(trivial);
    loop->addSource(timer);
    loop->loop();
  });
  trivialThread.join();
  ASSERT_TRUE(count == 1);
}
