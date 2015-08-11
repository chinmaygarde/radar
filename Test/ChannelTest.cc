// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <gtest/gtest.h>
#include <Core/Core.h>

#include <thread>

TEST(ChannelTest, SimpleInitialization) {
  rl::Channel channel;
  ASSERT_TRUE(channel.source() != nullptr);
}

TEST(ChannelTest, TestSimpleReads) {
  rl::Channel channel;

  rl::Latch latch(1);

  std::thread thread([&] {
    auto looper = rl::Looper::Current();
    ASSERT_TRUE(looper != nullptr);

    auto source = channel.source();
    ASSERT_TRUE(looper->addSource(source));

    channel.setMessagesReceivedCallback([&](rl::Messages m) {
      ASSERT_TRUE(m.size() == 1);
      ASSERT_TRUE(looper == rl::Looper::Current());
      looper->terminate();
    });

    looper->loop([&] { latch.countDown(); });
  });

  latch.wait();

  rl::Messages messages;
  rl::Message m;

  char c = 'c';
  ASSERT_TRUE(m.encode(c));
  messages.emplace_back(std::move(m));
  ASSERT_TRUE(channel.sendMessages(std::move(messages)));

  thread.join();
}

TEST(ChannelTest, TestSimpleReadContents) {
  rl::Channel channel;

  rl::Latch latch(1);

  size_t sendSize = 0;

  std::thread thread([&] {
    auto looper = rl::Looper::Current();
    ASSERT_TRUE(looper != nullptr);

    auto source = channel.source();
    ASSERT_TRUE(looper->addSource(source));

    channel.setMessagesReceivedCallback([&](rl::Messages messages) {
      ASSERT_TRUE(messages.size() == 1);
      ASSERT_TRUE(looper == rl::Looper::Current());

      auto& m = messages[0];

      char c = 'a';
      int d = 22;
      ASSERT_TRUE(m.decode(c));
      ASSERT_TRUE(m.decode(d));
      ASSERT_TRUE(m.size() == sendSize);
      ASSERT_TRUE(m.sizeRead() == sendSize);
      ASSERT_TRUE(c == 'c');
      ASSERT_TRUE(d == 222);

      looper->terminate();
    });

    looper->loop([&] { latch.countDown(); });
  });

  latch.wait();

  rl::Messages messages;
  rl::Message m;

  char c = 'c';
  int d = 222;
  ASSERT_TRUE(m.encode(c));
  ASSERT_TRUE(m.encode(d));

  sendSize = m.size();

  messages.emplace_back(std::move(m));
  ASSERT_TRUE(channel.sendMessages(std::move(messages)));

  thread.join();
}
