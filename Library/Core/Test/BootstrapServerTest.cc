// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <Core/BootstrapServer.h>
#include <Core/Core.h>
#include <TestRunner/TestRunner.h>
#include <thread>

TEST(BootstrapServerTest, NotFound) {
  rl::core::BootstrapServer daemon;

  auto foundSample = rl::core::BootstrapServer::Acquire("sample");
  ASSERT_EQ(foundSample, nullptr);

  auto foundNull = rl::core::BootstrapServer::Acquire("");
  ASSERT_EQ(foundNull, nullptr);
}

TEST(BootstrapServerTest, Advertise) {
  rl::core::BootstrapServer daemon;

  auto channel = std::make_shared<rl::core::Channel>();
  auto advertised = rl::core::BootstrapServer::Advertise("advertise", channel);
  ASSERT_EQ(advertised, rl::core::IOResult::Success);

  auto anotherChannel = std::make_shared<rl::core::Channel>();
  auto anotherAdvertised =
      rl::core::BootstrapServer::Advertise("", anotherChannel);
  ASSERT_EQ(anotherAdvertised, rl::core::IOResult::Failure);

  anotherAdvertised =
      rl::core::BootstrapServer::Advertise("advertise2", nullptr);
  ASSERT_EQ(anotherAdvertised, rl::core::IOResult::Failure);
}

TEST(BootstrapServerTest, AcquiredIsNotAdvertised) {
  rl::core::BootstrapServer daemon;

  /*
   *  The channel acquired after advertisement is not the channel being
   *  advertised. Instead, its an alias of the same.
   */
  auto channel = std::make_shared<rl::core::Channel>();
  auto advertised = rl::core::BootstrapServer::Advertise("advertise9", channel);
  ASSERT_EQ(advertised, rl::core::IOResult::Success);

  auto alias = rl::core::BootstrapServer::Acquire("advertise9");
  ASSERT_NE(channel, alias);
}

TEST(BootstrapServerTest, AdvertiseAgain) {
  rl::core::BootstrapServer daemon;

  auto name = "hello";

  auto channel = std::make_shared<rl::core::Channel>();
  auto advertised = rl::core::BootstrapServer::Advertise(name, channel);
  ASSERT_EQ(advertised, rl::core::IOResult::Success);

  advertised = rl::core::BootstrapServer::Advertise(name, channel);
  ASSERT_EQ(advertised, rl::core::IOResult::Failure);

  advertised = rl::core::BootstrapServer::Advertise(name, nullptr);
  ASSERT_EQ(advertised, rl::core::IOResult::Failure);
}

TEST(BootstrapServerTest, AcquireAndSendMessage) {
  rl::core::BootstrapServer daemon;

  auto name = "helloworld";
  auto channel = std::make_shared<rl::core::Channel>();
  auto advertised = rl::core::BootstrapServer::Advertise(name, channel);
  ASSERT_EQ(advertised, rl::core::IOResult::Success);

  rl::core::Latch ready(1);

  auto thread = std::thread([&]() {
    rl::core::thread::SetName("listener");
    auto advertised = rl::core::BootstrapServer::Acquire(name);
    ASSERT_NE(advertised, nullptr);

    advertised->setMessageCallback(
        [&](rl::core::Message message, rl::core::Namespace* ns) {
          auto a = 'x';
          ASSERT_EQ(message.decode(a, ns), true);
          ASSERT_EQ(a, 'a');
          ASSERT_EQ(message.readCompleted(), true);
          rl::core::EventLoop::Current()->terminate();
        });

    ASSERT_EQ(rl::core::EventLoop::Current()->addSource(advertised->source()),
              true);
    rl::core::EventLoop::Current()->loop([&]() { ready.countDown(); });
  });

  ready.wait();

  rl::core::Message message;
  ASSERT_TRUE(message.encode('a'));
  rl::core::Messages messages;
  messages.emplace_back(std::move(message));
  ASSERT_EQ(channel->sendMessages(std::move(messages)),
            rl::core::IOResult::Success);

  thread.join();
}
