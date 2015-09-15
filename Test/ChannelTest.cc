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

static bool MemorySetOrCheckPattern(uint8_t* buffer,
                                    size_t size,
                                    bool setOrCheck) {
  auto pattern4 = reinterpret_cast<const uint8_t*>("dErP");
  uint8_t* start = buffer;
  uint8_t* p = buffer;

  while ((start + size) - p >= 4) {
    if (setOrCheck) {
      bcopy(pattern4, p, 4);
    } else {
      if (memcmp(pattern4, p, 4) != 0) {
        return false;
      }
    }
    p += 4;
  }

  if ((start + size) - p != 0) {
    if (setOrCheck) {
      bcopy(pattern4, p, (start + size) - p);
    } else {
      if (memcmp(pattern4, p, (start + size) - p) != 0) {
        return false;
      }
    }
  }

  return true;
}

TEST(ChannelTest, TestSimpleReads) {
  rl::Channel channel;

  rl::Latch latch(1);

  std::thread thread([&] {
    auto loop = rl::EventLoop::Current();
    ASSERT_TRUE(loop != nullptr);

    auto source = channel.source();
    ASSERT_TRUE(loop->addSource(source));

    channel.setMessagesReceivedCallback([&](rl::Messages m) {
      ASSERT_TRUE(m.size() == 1);
      ASSERT_TRUE(loop == rl::EventLoop::Current());
      loop->terminate();
    });

    loop->loop([&] { latch.countDown(); });
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

TEST(ChannelTest, TestLargeReadWrite) {
  rl::Channel channel;

  rl::Latch latch(1);

  size_t sizeWritten = 0;

  std::thread thread([&] {
    auto loop = rl::EventLoop::Current();
    ASSERT_TRUE(loop != nullptr);

    auto source = channel.source();
    ASSERT_TRUE(loop->addSource(source));

    channel.setMessagesReceivedCallback([&](rl::Messages m) {
      ASSERT_TRUE(m.size() == 1);
      ASSERT_TRUE(m[0].size() == sizeWritten);
      ASSERT_TRUE(
          MemorySetOrCheckPattern(m[0].data(), sizeWritten, false /* check */));
      ASSERT_TRUE(loop == rl::EventLoop::Current());
      loop->terminate();
    });

    loop->loop([&] { latch.countDown(); });
  });

  latch.wait();

  rl::Messages messages;
  rl::Message m;

  const auto rawSize = 50000003;
  ASSERT_TRUE(MemorySetOrCheckPattern(m.encodeRawUnsafe(rawSize), rawSize,
                                      true /* set */));

  sizeWritten = m.size();
  messages.emplace_back(std::move(m));
  ASSERT_TRUE(channel.sendMessages(std::move(messages)));

  thread.join();
}

TEST(ChannelTest, TestSimpleReadContents) {
  rl::Channel channel;

  rl::Latch latch(1);

  size_t sendSize = 0;

  std::thread thread([&] {
    auto loop = rl::EventLoop::Current();
    ASSERT_TRUE(loop != nullptr);

    auto source = channel.source();
    ASSERT_TRUE(loop->addSource(source));

    channel.setMessagesReceivedCallback([&](rl::Messages messages) {
      ASSERT_TRUE(messages.size() == 1);
      ASSERT_TRUE(loop == rl::EventLoop::Current());

      auto& m = messages[0];

      char c = 'a';
      int d = 22;
      ASSERT_TRUE(m.decode(c));
      ASSERT_TRUE(m.decode(d));
      ASSERT_TRUE(m.size() == sendSize);
      ASSERT_TRUE(m.sizeRead() == sendSize);
      ASSERT_TRUE(c == 'c');
      ASSERT_TRUE(d == 222);

      loop->terminate();
    });

    loop->loop([&] { latch.countDown(); });
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
