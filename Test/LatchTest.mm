// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "RadarTest.h"

#include <Core/Core.h>
#include <thread>

RL_DECLARE_TEST_START(LatchTest)

TEST(LatchTest, SimpleLatch) {
  rl::core::Latch l(3);
  bool done[3]{false, false, false};

  std::thread one([&]() {
    std::this_thread::sleep_for(rl::core::ClockDurationMicro(20));
    done[0] = true;
    l.countDown();
  });

  std::thread two([&]() {
    std::this_thread::sleep_for(rl::core::ClockDurationMicro(20));
    done[1] = true;
    l.countDown();
  });

  std::thread three([&]() {
    std::this_thread::sleep_for(rl::core::ClockDurationMicro(20));
    done[2] = true;
    l.countDown();
  });

  l.wait();

  ASSERT_TRUE(done[0] == true);
  ASSERT_TRUE(done[1] == true);
  ASSERT_TRUE(done[2] == true);

  one.join();
  two.join();
  three.join();
}

TEST(LatchTest, AutoLatch) {
  std::thread one, two, three;
  bool done[3]{false, false, false};
  {
    rl::core::AutoLatch l(3);

    one = std::thread([&]() {
      std::this_thread::sleep_for(rl::core::ClockDurationMicro(20));
      done[0] = true;
      l.countDown();
    });

    two = std::thread([&]() {
      std::this_thread::sleep_for(rl::core::ClockDurationMicro(20));
      done[1] = true;
      l.countDown();
    });

    three = std::thread([&]() {
      std::this_thread::sleep_for(rl::core::ClockDurationMicro(20));
      done[2] = true;
      l.countDown();
    });
  }

  ASSERT_TRUE(done[0] == true);
  ASSERT_TRUE(done[1] == true);
  ASSERT_TRUE(done[2] == true);

  one.join();
  two.join();
  three.join();
}

RL_DECLARE_TEST_END