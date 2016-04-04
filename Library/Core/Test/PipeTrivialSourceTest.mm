// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <TestRunner/TestRunner.h>

#include <Core/Core.h>
#include "PipeTrivialSource.h"

#include <thread>

#if RL_CHANNELS != RL_CHANNELS_INPROCESS

RL_DECLARE_TEST_START(PipeTrivialSourceTest)

TEST(PipeTrivialSourceTest, SimpleInitialization) {
  auto pipeSource = rl::core::MakePipeBasedTrivialSource();
  rl::core::Latch ready(1);

  std::thread thread([&]() {
    auto loop = rl::core::EventLoop::Current();
    loop->addSource(pipeSource);
    loop->loop([&]() { ready.countDown(); });
  });

  ready.wait();

  auto readOnPipe = 0;
  pipeSource->setWakeFunction([&](rl::core::IOResult) {
    readOnPipe++;
    rl::core::EventLoop::Current()->terminate();
  });

  for (int i = 0; i < 10; i++) {
    ASSERT_EQ(pipeSource->writer()(pipeSource->writeHandle()),
              rl::core::IOResult::Success);
  }

  thread.join();

  ASSERT_EQ(readOnPipe, 1);
}

RL_DECLARE_TEST_END

#endif  // RL_CHANNELS != RL_CHANNELS_INPROCESS
