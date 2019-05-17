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

TEST(EventLoopObserverTest, SimpleLoopObserver) {
  auto beforeSleep = 0;
  auto afterSleep = 0;
  std::thread thread([&] {
    auto loop = EventLoop::Current();
    ASSERT_NE(loop, nullptr);
    auto obs = std::make_shared<EventLoopObserver>(
        [&](EventLoopObserver::Activity activity) {
          switch (activity) {
            case EventLoopObserver::Activity::BeforeSleep:
              beforeSleep++;
              EventLoop::Current()->terminate();
              break;
            case EventLoopObserver::Activity::AfterSleep:
              afterSleep++;
              break;
          }
        });
    ASSERT_EQ(loop->addObserver(obs, EventLoopObserver::Activity::BeforeSleep),
              true);
    ASSERT_EQ(loop->addObserver(obs, EventLoopObserver::Activity::AfterSleep),
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
    auto loop = EventLoop::Current();
    ASSERT_NE(loop, nullptr);
    auto obs1 = std::make_shared<EventLoopObserver>(
        [&](EventLoopObserver::Activity activity) {
          switch (activity) {
            case EventLoopObserver::Activity::BeforeSleep:
              ASSERT_EQ(orderCount, 0);
              orderCount++;
              break;
            case EventLoopObserver::Activity::AfterSleep:
              ASSERT_EQ(true, false);
              break;
          }
        },
        1 /* priority */);
    auto obs2 = std::make_shared<EventLoopObserver>(
        [&](EventLoopObserver::Activity activity) {
          switch (activity) {
            case EventLoopObserver::Activity::BeforeSleep:
              ASSERT_EQ(orderCount, 1);
              orderCount++;
              break;
            case EventLoopObserver::Activity::AfterSleep:
              ASSERT_EQ(true, false);
              break;
          }
        },
        2 /* priority */);
    auto obs3 = std::make_shared<EventLoopObserver>(
        [&](EventLoopObserver::Activity activity) {
          switch (activity) {
            case EventLoopObserver::Activity::BeforeSleep:
              ASSERT_EQ(orderCount, 2);
              orderCount++;
              break;
            case EventLoopObserver::Activity::AfterSleep:
              ASSERT_EQ(true, false);
              break;
          }
        },
        3 /* priority */);
    auto obs4 = std::make_shared<EventLoopObserver>(
        [&](EventLoopObserver::Activity activity) {
          switch (activity) {
            case EventLoopObserver::Activity::BeforeSleep:
              ASSERT_EQ(orderCount, 3);
              orderCount++;
              EventLoop::Current()->terminate();
              break;
            case EventLoopObserver::Activity::AfterSleep:
              ASSERT_EQ(true, false);
              break;
          }
        },
        4 /* priority */);

    /*
     *  The observers are added in a random order
     */
    ASSERT_EQ(loop->addObserver(obs1, EventLoopObserver::Activity::BeforeSleep),
              true);
    ASSERT_EQ(loop->addObserver(obs4, EventLoopObserver::Activity::BeforeSleep),
              true);
    ASSERT_EQ(loop->addObserver(obs3, EventLoopObserver::Activity::BeforeSleep),
              true);
    ASSERT_EQ(loop->addObserver(obs2, EventLoopObserver::Activity::BeforeSleep),
              true);

    loop->loop();
  });

  thread.join();
  ASSERT_EQ(orderCount, 4);
}

TEST_SLOW(EventLoopObserverTest, SingleWakeServicesAllReads) {
  auto beforeSleep = 0;
  auto afterSleep = 0;
  auto reads = 0;

  EventLoopSource::WakeFunction wake = [&](IOResult res) {
    reads++;
    if (reads == 5) {
      EventLoop::Current()->terminate();
    }
  };

  auto trivial1 = EventLoopSource::Trivial();
  trivial1->setWakeFunction(wake);

  auto trivial2 = EventLoopSource::Trivial();
  trivial2->setWakeFunction(wake);

  auto trivial3 = EventLoopSource::Trivial();
  trivial3->setWakeFunction(wake);

  auto trivial4 = EventLoopSource::Trivial();
  trivial4->setWakeFunction(wake);

  auto trivial5 = EventLoopSource::Trivial();
  trivial5->setWakeFunction(wake);

  /*
   *  All handles have data waiting on them
   */
  trivial1->writer()(trivial1->handles().writeHandle);
  trivial2->writer()(trivial2->handles().writeHandle);
  trivial3->writer()(trivial3->handles().writeHandle);
  trivial4->writer()(trivial4->handles().writeHandle);
  trivial5->writer()(trivial5->handles().writeHandle);

  std::this_thread::sleep_for(std::chrono::milliseconds(5));

  std::thread thread([&] {
    auto loop = EventLoop::Current();
    ASSERT_NE(loop, nullptr);
    auto obs = std::make_shared<EventLoopObserver>(
        [&](EventLoopObserver::Activity activity) {
          switch (activity) {
            case EventLoopObserver::Activity::BeforeSleep:
              beforeSleep++;
              break;
            case EventLoopObserver::Activity::AfterSleep:
              afterSleep++;
              break;
          }
        });
    ASSERT_EQ(loop->addObserver(obs, EventLoopObserver::Activity::BeforeSleep),
              true);
    ASSERT_EQ(loop->addObserver(obs, EventLoopObserver::Activity::AfterSleep),
              true);

    loop->addSource(trivial1);
    loop->addSource(trivial2);
    loop->addSource(trivial3);
    loop->addSource(trivial4);
    loop->addSource(trivial5);

    loop->loop();
  });

  thread.join();

  ASSERT_EQ(beforeSleep, 2);
  ASSERT_EQ(afterSleep, 2);
  ASSERT_EQ(reads, 5);
}

}  // namespace testing
}  // namespace core
}  // namespace rl
