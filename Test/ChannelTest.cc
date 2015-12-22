// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <Core/Core.h>
#include <gtest/gtest.h>

#include <thread>

TEST(ChannelTest, SimpleInitialization) {
  rl::core::Channel channel;
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
      memmove(p, pattern4, 4);
    } else {
      if (memcmp(pattern4, p, 4) != 0) {
        return false;
      }
    }
    p += 4;
  }

  if ((start + size) - p != 0) {
    if (setOrCheck) {
      memmove(p, pattern4, (start + size) - p);
    } else {
      if (memcmp(pattern4, p, (start + size) - p) != 0) {
        return false;
      }
    }
  }

  return true;
}

TEST(ChannelTest, TestSimpleReads) {
  rl::core::Channel channel;

  rl::core::Latch latch(1);

  std::thread thread([&] {
    auto loop = rl::core::EventLoop::Current();
    ASSERT_TRUE(loop != nullptr);

    auto source = channel.source();
    ASSERT_TRUE(loop->addSource(source));

    channel.setMessageCallback([&](rl::core::Message m) {
      ASSERT_TRUE(loop == rl::core::EventLoop::Current());
      loop->terminate();
    });

    loop->loop([&] { latch.countDown(); });
  });

  latch.wait();

  rl::core::Messages messages;
  rl::core::Message m;

  char c = 'c';
  ASSERT_TRUE(m.encode(c));
  messages.emplace_back(std::move(m));
  ASSERT_TRUE(channel.sendMessages(std::move(messages)));

  thread.join();
}

TEST(ChannelTest, TestLargeReadWrite) {
  rl::core::Channel channel;

  rl::core::Latch latch(1);

  size_t sizeWritten = 0;

  std::thread thread([&] {
    auto loop = rl::core::EventLoop::Current();
    ASSERT_TRUE(loop != nullptr);

    auto source = channel.source();
    ASSERT_TRUE(loop->addSource(source));

    channel.setMessageCallback([&](rl::core::Message m) {
      ASSERT_TRUE(m.size() == sizeWritten);
      ASSERT_TRUE(
          MemorySetOrCheckPattern(m.data(), sizeWritten, false /* check */));
      ASSERT_TRUE(loop == rl::core::EventLoop::Current());
      loop->terminate();
    });

    loop->loop([&] { latch.countDown(); });
  });

  latch.wait();

  rl::core::Messages messages;
  rl::core::Message m;

  const auto rawSize = 50000003;
  ASSERT_TRUE(MemorySetOrCheckPattern(m.encodeRaw<uint8_t>(rawSize), rawSize,
                                      true /* set */));

  sizeWritten = m.size();
  messages.emplace_back(std::move(m));
  ASSERT_TRUE(channel.sendMessages(std::move(messages)));

  thread.join();
}

TEST(ChannelTest, TestSimpleReadContents) {
  rl::core::Channel channel;

  rl::core::Latch latch(1);

  size_t sendSize = 0;

  std::thread thread([&] {
    auto loop = rl::core::EventLoop::Current();
    ASSERT_TRUE(loop != nullptr);

    auto source = channel.source();
    ASSERT_TRUE(loop->addSource(source));

    channel.setMessageCallback([&](rl::core::Message m) {
      ASSERT_TRUE(loop == rl::core::EventLoop::Current());

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

  rl::core::Messages messages;
  rl::core::Message m;

  char c = 'c';
  int d = 222;
  ASSERT_TRUE(m.encode(c));
  ASSERT_TRUE(m.encode(d));

  sendSize = m.size();

  messages.emplace_back(std::move(m));
  ASSERT_TRUE(channel.sendMessages(std::move(messages)));

  thread.join();
}
