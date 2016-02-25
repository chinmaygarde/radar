// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "RadarTest.h"

#include <Core/Core.h>

#include <Core/BootstrapServer.h>

#include <thread>

RL_DECLARE_TEST_START(BootstrapServerTest)

TEST(BootstrapServerTest, NotFound) {
  auto found = rl::core::BootstrapServerAcquireAdvertised("sample");
  ASSERT_EQ(found, nullptr);

  found = rl::core::BootstrapServerAcquireAdvertised("");
  ASSERT_EQ(found, nullptr);
}

TEST(BootstrapServerTest, Advertise) {
  auto channel = std::make_shared<rl::core::Channel>();
  auto advertised = rl::core::BootstrapServerAdvertise("advertise", channel);
  ASSERT_EQ(advertised, rl::core::IOResult::Success);

  auto anotherChannel = std::make_shared<rl::core::Channel>();
  auto anotherAdvertised =
      rl::core::BootstrapServerAdvertise("", anotherChannel);
  ASSERT_EQ(anotherAdvertised, rl::core::IOResult::Failure);

  anotherAdvertised = rl::core::BootstrapServerAdvertise("advertise2", nullptr);
  ASSERT_EQ(anotherAdvertised, rl::core::IOResult::Failure);
}

TEST(BootstrapServerTest, AcquiredIsNotAdvertised) {
  /*
   *  The channel acquired after advertisement is not the channel being
   *  advertised. Instead, its an alias of the same.
   */
  auto channel = std::make_shared<rl::core::Channel>();
  auto advertised = rl::core::BootstrapServerAdvertise("advertise9", channel);
  ASSERT_EQ(advertised, rl::core::IOResult::Success);

  auto alias = rl::core::BootstrapServerAcquireAdvertised("advertise9");
  ASSERT_NE(channel, alias);
}

TEST(BootstrapServerTest, AdvertiseAgain) {
  auto name = "hello";

  auto channel = std::make_shared<rl::core::Channel>();
  auto advertised = rl::core::BootstrapServerAdvertise(name, channel);
  ASSERT_EQ(advertised, rl::core::IOResult::Success);

  advertised = rl::core::BootstrapServerAdvertise(name, channel);
  ASSERT_EQ(advertised, rl::core::IOResult::Failure);

  advertised = rl::core::BootstrapServerAdvertise(name, nullptr);
  ASSERT_EQ(advertised, rl::core::IOResult::Failure);
}

TEST(BootstrapServerTest, Acquire) {
  auto name = "helloworld";
  auto channel = std::make_shared<rl::core::Channel>();
  auto advertised = rl::core::BootstrapServerAdvertise(name, channel);
  ASSERT_EQ(advertised, rl::core::IOResult::Success);

  rl::core::Latch ready(1);

  auto thread = std::thread([&]() {
    rl::core::thread::SetName("listener");
    auto advertised = rl::core::BootstrapServerAcquireAdvertised(name);
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
  message.encode('a');
  rl::core::Messages messages;
  messages.emplace_back(std::move(message));
  ASSERT_EQ(channel->sendMessages(std::move(messages)),
            rl::core::IOResult::Success);

  thread.join();
}

RL_DECLARE_TEST_END
