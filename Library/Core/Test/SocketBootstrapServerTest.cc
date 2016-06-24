// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <Core/Config.h>

#if 0

#if RL_CHANNELS == RL_CHANNELS_SOCKET

#include <TestRunner/TestRunner.h>

#include <Core/Core.h>
#include <Core/SocketBootstrapServer.h>

#include <thread>



TEST(SocketBootstrapServerTest, InitializeFromProvider) {
  rl::core::SocketBootstrapServer bootstrap;
  ASSERT_NE(bootstrap.source(), nullptr);
}

TEST(SocketBootstrapServerTest, ScheduleInLoop) {
  auto termination = rl::core::EventLoopSource::Trivial();

  termination->setWakeFunction([](rl::core::IOResult result) {
    rl::core::EventLoop::Current()->terminate();
  });

  bool started = false;

  rl::core::Latch readyLatch(1);

  std::thread server([&]() {
    rl::core::SocketBootstrapServer bootstrap;
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

#endif  // RL_CHANNELS == RL_CHANNELS_SOCKET

#endif
