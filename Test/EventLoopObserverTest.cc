// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <stdio.h>

#include <Core/Core.h>
#include <gtest/gtest.h>
#include <thread>

TEST(EventLoopObserverTest, SimpleLoopObserver) {
  auto beforeSleep = 0;
  auto afterSleep = 0;
  std::thread thread([&] {
    auto loop = rl::core::EventLoop::Current();
    ASSERT_NE(loop, nullptr);
    auto obs = std::make_shared<rl::core::EventLoopObserver>(
        [&](rl::core::EventLoopObserver::Activity activity) {
          switch (activity) {
            case rl::core::EventLoopObserver::Activity::BeforeSleep:
              beforeSleep++;
              rl::core::EventLoop::Current()->terminate();
              break;
            case rl::core::EventLoopObserver::Activity::AfterSleep:
              afterSleep++;
              break;
          }
        });
    ASSERT_EQ(loop->addObserver(
                  obs, rl::core::EventLoopObserver::Activity::BeforeSleep),
              true);
    ASSERT_EQ(loop->addObserver(
                  obs, rl::core::EventLoopObserver::Activity::AfterSleep),
              true);
    loop->loop();
  });

  thread.join();
  ASSERT_EQ(beforeSleep, 1);
  ASSERT_EQ(afterSleep, 1);
}

TEST(EventLoopObserverTest, LoopObserverPriorities) {
  auto orderCount = 0;
  std::thread thread([&] {
    auto loop = rl::core::EventLoop::Current();
    ASSERT_NE(loop, nullptr);
    auto obs1 = std::make_shared<rl::core::EventLoopObserver>(
        [&](rl::core::EventLoopObserver::Activity activity) {
          switch (activity) {
            case rl::core::EventLoopObserver::Activity::BeforeSleep:
              ASSERT_EQ(orderCount, 0);
              orderCount++;
              break;
            case rl::core::EventLoopObserver::Activity::AfterSleep:
              ASSERT_EQ(true, false);
              break;
          }
        },
        1 /* priority */);
    auto obs2 = std::make_shared<rl::core::EventLoopObserver>(
        [&](rl::core::EventLoopObserver::Activity activity) {
          switch (activity) {
            case rl::core::EventLoopObserver::Activity::BeforeSleep:
              ASSERT_EQ(orderCount, 1);
              orderCount++;
              break;
            case rl::core::EventLoopObserver::Activity::AfterSleep:
              ASSERT_EQ(true, false);
              break;
          }
        },
        2 /* priority */);
    auto obs3 = std::make_shared<rl::core::EventLoopObserver>(
        [&](rl::core::EventLoopObserver::Activity activity) {
          switch (activity) {
            case rl::core::EventLoopObserver::Activity::BeforeSleep:
              ASSERT_EQ(orderCount, 2);
              orderCount++;
              break;
            case rl::core::EventLoopObserver::Activity::AfterSleep:
              ASSERT_EQ(true, false);
              break;
          }
        },
        3 /* priority */);
    auto obs4 = std::make_shared<rl::core::EventLoopObserver>(
        [&](rl::core::EventLoopObserver::Activity activity) {
          switch (activity) {
            case rl::core::EventLoopObserver::Activity::BeforeSleep:
              ASSERT_EQ(orderCount, 3);
              orderCount++;
              rl::core::EventLoop::Current()->terminate();
              break;
            case rl::core::EventLoopObserver::Activity::AfterSleep:
              ASSERT_EQ(true, false);
              break;
          }
        },
        4 /* priority */);

    /*
     *  The observers are added in a random order
     */
    ASSERT_EQ(loop->addObserver(
                  obs1, rl::core::EventLoopObserver::Activity::BeforeSleep),
              true);
    ASSERT_EQ(loop->addObserver(
                  obs4, rl::core::EventLoopObserver::Activity::BeforeSleep),
              true);
    ASSERT_EQ(loop->addObserver(
                  obs3, rl::core::EventLoopObserver::Activity::BeforeSleep),
              true);
    ASSERT_EQ(loop->addObserver(
                  obs2, rl::core::EventLoopObserver::Activity::BeforeSleep),
              true);

    loop->loop();
  });

  thread.join();
  ASSERT_EQ(orderCount, 4);
}
