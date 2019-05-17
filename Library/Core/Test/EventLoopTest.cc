/*
 *  This source file is part of the Radar project.
 *  Licensed under the MIT License. See LICENSE file for details.
 */

#include <Core/EventLoop.h>
#include <TestRunner/TestRunner.h>
#include <thread>

namespace rl {
namespace core {
namespace testing {

TEST(EventLoopTest, CurrentEventLoopAccess) {
  auto loop = EventLoop::Current();
  ASSERT_TRUE(loop != nullptr);
  ASSERT_TRUE(EventLoop::Current() == loop);
}

TEST(EventLoopTest, EventLoopOnAnotherThread) {
  EventLoop* loop1 = EventLoop::Current();
  EventLoop* loop2 = nullptr;

  std::thread thread([&] { loop2 = EventLoop::Current(); });

  thread.join();

  ASSERT_TRUE(loop1 != loop2);
}

TEST(EventLoopTest, SimpleLoop) {
  std::thread thread([&] {
    auto outer = EventLoop::Current();

    bool terminatedFromInner = false;

    std::thread innerThread([&] {
      std::this_thread::sleep_for(ClockDurationMilli(1));
      terminatedFromInner = true;
      outer->terminate();
      ASSERT_TRUE(true);
    });

    /*
     *  Keep looping till the inner thread terminates the loop.
     */
    outer->loop();

    innerThread.join();

    ASSERT_TRUE(terminatedFromInner);
  });

  thread.join();
  ASSERT_TRUE(true);
}

TEST_SLOW(EventLoopTest, Timer) {
  std::thread timerThread([&] {
    auto loop = EventLoop::Current();

    Clock clock;

    const auto start = clock.now();

    auto timer = EventLoopSource::Timer(ClockDurationMilli(5));

    /*
     *  This test is extremely brittle :/
     */
    timer->setWakeFunction([&](IOResult cause) {
      auto duration =
          std::chrono::duration_cast<ClockDurationMilli>(clock.now() - start);
      EventLoop::Current()->terminate();
      ASSERT_TRUE(duration.count() >= 4 && duration.count() <= 6);
    });

    loop->addSource(timer);

    loop->loop();
  });

  timerThread.join();
}

TEST_SLOW(EventLoopTest, TimerRepetition) {
  size_t count = 0;

  std::thread timerThread([&count] {
    auto loop = EventLoop::Current();

    auto timer = EventLoopSource::Timer(ClockDurationMilli(1));

    timer->setWakeFunction([&count, loop](IOResult cause) {
      count++;

      if (count == 5) {
        loop->terminate();
      }
    });

    loop->addSource(timer);

    loop->loop();
  });

  timerThread.join();

  ASSERT_EQ(count, 5u);
}

TEST_SLOW(EventLoopTest, TrivialTriggerFiresOnces) {
  size_t count = 0;
  std::thread trivialThread([&count] {
    auto loop = EventLoop::Current();
    auto trivial = EventLoopSource::Trivial();
    trivial->setWakeFunction([&count](IOResult cause) { count++; });

    /**
     *  Fire the trivial source multiple times. We need to assert that the
     *  callback is only triggered once. Admittedly, this is a bit tricky to
     *  test in a non brittle manner.
     */
    trivial->writer()(trivial->handles().writeHandle);
    trivial->writer()(trivial->handles().writeHandle);
    trivial->writer()(trivial->handles().writeHandle);
    trivial->writer()(trivial->handles().writeHandle);
    trivial->writer()(trivial->handles().writeHandle);

    auto timer = EventLoopSource::Timer(ClockDurationMilli(5));

    timer->setWakeFunction(
        [](IOResult cause) { EventLoop::Current()->terminate(); });

    loop->addSource(trivial);
    loop->addSource(timer);
    loop->loop();
  });
  trivialThread.join();
  ASSERT_EQ(count, 1u);
}

}  // namespace testing
}  // namespace core
}  // namespace rl
