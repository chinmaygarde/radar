// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <Core/Config.h>

#include "RadarTest.h"

#include <Core/Core.h>
#include <Core/BootstrapSocketServer.h>

#include <thread>

RL_DECLARE_TEST_START(BootstrapSocketServerTest)

TEST(BootstrapSocketServerTest, InitializeFromProvider) {
  rl::core::BootstrapSocketServer bootstrap;
  ASSERT_NE(bootstrap.source(), nullptr);
}

TEST(BootstrapSocketServerTest, ScheduleInLoop) {
  auto termination = rl::core::EventLoopSource::Trivial();

  termination->setWakeFunction([](rl::core::IOResult result) {
    rl::core::EventLoop::Current()->terminate();
  });

  bool started = false;

  rl::core::Latch readyLatch(1);

  std::thread server([&]() {
    rl::core::BootstrapSocketServer bootstrap;
    ASSERT_NE(bootstrap.source(), nullptr);

    auto loop = rl::core::EventLoop::Current();
    loop->addSource(bootstrap.source());
    loop->addSource(termination);
    loop->loop([&]() {
      started = true;
      readyLatch.countDown();
    });
  });

  readyLatch.wait();
  termination->writer()(termination->writeHandle());

  server.join();

  ASSERT_EQ(started, true);
}

RL_DECLARE_TEST_END
