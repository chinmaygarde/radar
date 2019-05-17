/*
 *  This source file is part of the Radar project.
 *  Licensed under the MIT License. See LICENSE file for details.
 */

#include <Core/BootstrapServer.h>
#include <Core/Latch.h>
#include <Core/Thread.h>
#include <TestRunner/TestRunner.h>
#include <thread>

namespace rl {
namespace core {
namespace testing {

TEST(BootstrapServerTest, NotFound) {
  BootstrapServer daemon;

  auto foundSample = BootstrapServer::Acquire("sample");
  ASSERT_EQ(foundSample, nullptr);

  auto foundNull = BootstrapServer::Acquire("");
  ASSERT_EQ(foundNull, nullptr);
}

TEST(BootstrapServerTest, Advertise) {
  BootstrapServer daemon;

  auto channel = std::make_shared<Channel>();
  auto advertised = BootstrapServer::Advertise("advertise", channel);
  ASSERT_EQ(advertised, IOResult::Success);

  auto anotherChannel = std::make_shared<Channel>();
  auto anotherAdvertised = BootstrapServer::Advertise("", anotherChannel);
  ASSERT_EQ(anotherAdvertised, IOResult::Failure);

  anotherAdvertised = BootstrapServer::Advertise("advertise2", nullptr);
  ASSERT_EQ(anotherAdvertised, IOResult::Failure);
}

TEST(BootstrapServerTest, AcquiredIsNotAdvertised) {
  BootstrapServer daemon;

  /*
   *  The channel acquired after advertisement is not the channel being
   *  advertised. Instead, its an alias of the same.
   */
  auto channel = std::make_shared<Channel>();
  auto advertised = BootstrapServer::Advertise("advertise9", channel);
  ASSERT_EQ(advertised, IOResult::Success);

  auto alias = BootstrapServer::Acquire("advertise9");
  ASSERT_NE(channel, alias);
}

TEST(BootstrapServerTest, AdvertiseAgain) {
  BootstrapServer daemon;

  auto name = "hello";

  auto channel = std::make_shared<Channel>();
  auto advertised = BootstrapServer::Advertise(name, channel);
  ASSERT_EQ(advertised, IOResult::Success);

  advertised = BootstrapServer::Advertise(name, channel);
  ASSERT_EQ(advertised, IOResult::Failure);

  advertised = BootstrapServer::Advertise(name, nullptr);
  ASSERT_EQ(advertised, IOResult::Failure);
}

TEST(BootstrapServerTest, AcquireAndSendMessage) {
  BootstrapServer daemon;

  auto name = "helloworld";
  auto channel = std::make_shared<Channel>();
  auto advertised = BootstrapServer::Advertise(name, channel);
  ASSERT_EQ(advertised, IOResult::Success);

  Latch ready(1);

  auto thread = std::thread([&]() {
    thread::SetName("listener");
    auto advertised = BootstrapServer::Acquire(name);
    ASSERT_NE(advertised, nullptr);

    advertised->setMessageCallback([&](Message message, Namespace* ns) {
      auto a = 'x';
      ASSERT_EQ(message.decode(a, ns), true);
      ASSERT_EQ(a, 'a');
      ASSERT_EQ(message.readCompleted(), true);
      EventLoop::Current()->terminate();
    });

    ASSERT_EQ(EventLoop::Current()->addSource(advertised->source()), true);
    EventLoop::Current()->loop([&]() { ready.countDown(); });
  });

  ready.wait();

  Message message;
  ASSERT_TRUE(message.encode('a'));
  Messages messages;
  messages.emplace_back(std::move(message));
  ASSERT_EQ(channel->sendMessages(std::move(messages)), IOResult::Success);

  thread.join();
}

}  // namespace testing
}  // namespace core
}  // namespace rl
