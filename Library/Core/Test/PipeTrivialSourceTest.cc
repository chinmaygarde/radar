/*
 *  This source file is part of the Radar project.
 *  Licensed under the MIT License. See LICENSE file for details.
 */

#include <Core/EventLoop.h>
#include <Core/Latch.h>
#include <TestRunner/TestRunner.h>
#include <thread>
#include "PipeTrivialSource.h"

#if RL_CHANNELS != RL_CHANNELS_INPROCESS

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
    ASSERT_EQ(pipeSource->writer()(pipeSource->handles().writeHandle),
              rl::core::IOResult::Success);
  }

  thread.join();

  ASSERT_EQ(readOnPipe, 1);
}

#endif  // RL_CHANNELS != RL_CHANNELS_INPROCESS
