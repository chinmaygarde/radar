/*
 *  This source file is part of the Radar project.
 *  Licensed under the MIT License. See LICENSE file for details.
 */

#include <thread>

#include <Core/EventLoop.h>
#include <Core/Latch.h>
#include <TestRunner/TestRunner.h>

#include "PipeTrivialSource.h"

namespace rl {
namespace core {
namespace testing {

#if RL_CHANNELS != RL_CHANNELS_INPROCESS

TEST(PipeTrivialSourceTest, SimpleInitialization) {
  auto pipeSource = MakePipeBasedTrivialSource();
  Latch ready(1);

  std::thread thread([&]() {
    auto loop = EventLoop::Current();
    loop->addSource(pipeSource);
    loop->loop([&]() { ready.countDown(); });
  });

  ready.wait();

  auto readOnPipe = 0;
  pipeSource->setWakeFunction([&](IOResult) {
    readOnPipe++;
    EventLoop::Current()->terminate();
  });

  for (int i = 0; i < 10; i++) {
    ASSERT_EQ(pipeSource->writer()(pipeSource->handles().writeHandle),
              IOResult::Success);
  }

  thread.join();

  ASSERT_EQ(readOnPipe, 1);
}

#endif  // RL_CHANNELS != RL_CHANNELS_INPROCESS

}  // namespace testing
}  // namespace core
}  // namespace rl
