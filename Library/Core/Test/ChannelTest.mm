// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <TestRunner/TestRunner.h>

#include <Core/Core.h>
#include <Core/Stopwatch.h>

#include <thread>

static bool MemorySetOrCheckPattern(uint8_t* buffer,
                                    size_t size,
                                    bool setOrCheck) {
  if (buffer == nullptr) {
    return false;
  }

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

TEST(ChannelTest, SimpleInitialization) {
  rl::core::Channel channel;
  ASSERT_TRUE(channel.source() != nullptr);
}

TEST_SLOW(ChannelTest, ReadTimeout) {
  auto ms = 50;

  rl::core::Channel channel;
  rl::instrumentation::Stopwatch stopwatch;

  {
    rl::instrumentation::AutoStopwatchLap lap(stopwatch);
    auto result =
        channel.readPendingMessageNow(rl::core::ClockDurationMilli(ms));
    ASSERT_EQ(result, rl::core::IOResult::Timeout);
  }

  ASSERT_GE(stopwatch.lastLap(), rl::core::ClockDurationMilli(ms));
}

#if RL_CHANNELS != RL_CHANNELS_MACH

TEST(ChannelTest, ChannelDeathRemovesChannelAliasesFromWaitsets) {
  rl::core::Channel channel;
  rl::core::Message::Attachment channelAttachment =
      channel.asMessageAttachment();

  bool aliasTerminated = false;
  rl::core::Latch readyLatch(1);
  std::thread waiter([&]() {
    rl::core::thread::SetName("waiter");
    auto loop = rl::core::EventLoop::Current();

    rl::core::Channel aliasChannel(channelAttachment);

    aliasChannel.setTerminationCallback([&](rl::core::Channel&) {
      aliasTerminated = true;
      ASSERT_EQ(loop, rl::core::EventLoop::Current());
      rl::core::EventLoop::Current()->terminate();
    });

    loop->addSource(aliasChannel.source());

    loop->loop([&]() { readyLatch.countDown(); });
  });

  readyLatch.wait();

  bool sourceTerminated = false;
  channel.setTerminationCallback(
      [&](rl::core::Channel&) { sourceTerminated = true; });

  channel.terminate();

  waiter.join();

  ASSERT_EQ(sourceTerminated, true);
  ASSERT_EQ(aliasTerminated, true);
}

#endif  // RL_CHANNELS != RL_CHANNELS_MACH

TEST(ChannelTest, TestSimpleReads) {
  rl::core::Channel channel;

  rl::core::Latch latch(1);

  std::thread thread([&] {
    auto loop = rl::core::EventLoop::Current();
    ASSERT_TRUE(loop != nullptr);

    auto source = channel.source();
    ASSERT_TRUE(loop->addSource(source));

    channel.setMessageCallback(
        [&](rl::core::Message m, rl::core::Namespace* ns) {
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
  ASSERT_EQ(channel.sendMessages(std::move(messages)),
            rl::core::IOResult::Success);

  thread.join();
}

TEST_SLOW(ChannelTest, TestLargeReadWrite) {
  rl::core::Channel channel;

  rl::core::Latch latch(1);

  const auto rawSize = 50000003;

  std::thread thread([&] {
    auto loop = rl::core::EventLoop::Current();
    ASSERT_TRUE(loop != nullptr);

    auto source = channel.source();
    ASSERT_TRUE(loop->addSource(source));

    channel.setMessageCallback(
        [&](rl::core::Message m, rl::core::Namespace* ns) {
          ASSERT_TRUE(m.size() == rawSize);
          ASSERT_TRUE(
              MemorySetOrCheckPattern(m.data(), rawSize, false /* check */));
          ASSERT_TRUE(loop == rl::core::EventLoop::Current());
          loop->terminate();
        });

    loop->loop([&] { latch.countDown(); });
  });

  latch.wait();

  rl::core::Messages messages;
  rl::core::Message m;

  ASSERT_TRUE(MemorySetOrCheckPattern(m.encodeRaw<uint8_t>(rawSize), rawSize,
                                      true /* set */));
  size_t sizeWritten = m.size();
  ASSERT_EQ(sizeWritten, rawSize);

  messages.emplace_back(std::move(m));
  ASSERT_EQ(channel.sendMessages(std::move(messages)),
            rl::core::IOResult::Success);

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

    channel.setMessageCallback(
        [&](rl::core::Message m, rl::core::Namespace* ns) {
          ASSERT_TRUE(loop == rl::core::EventLoop::Current());

          char c = 'a';
          int d = 22;
          ASSERT_TRUE(m.decode(c, nullptr));
          ASSERT_TRUE(m.decode(d, nullptr));
          ASSERT_EQ(m.size(), m.sizeRead());
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
  ASSERT_EQ(channel.sendMessages(std::move(messages)),
            rl::core::IOResult::Success);

  thread.join();
}

TEST(ChannelTest, ZeroTimeoutReadsDontHang) {
  rl::core::Channel channel;
  rl::core::Messages messages = channel.drainPendingMessages();
  ASSERT_EQ(messages.size(), 0);
}

TEST(ChannelTest, SendAttachmentsOverChannels) {
  rl::core::Channel chan;

  chan.setMessageCallback(
      [&](rl::core::Message message, rl::core::Namespace* ns) {
        rl::core::RawAttachment attachment;

        ASSERT_EQ(message.decode(attachment), true);

        rl::core::Channel consumer(std::move(attachment));

        /*
         *  There is only one attachment
         */
        ASSERT_EQ(message.decode(attachment), false);

        ASSERT_EQ(message.readCompleted(), true);

        rl::core::EventLoop::Current()->terminate();
      });

  rl::core::Latch latch(1);
  std::thread thread([&]() {
    auto loop = rl::core::EventLoop::Current();
    loop->addSource(chan.source());

    loop->loop([&]() { latch.countDown(); });
  });

  latch.wait();

  rl::core::Channel other;

  rl::core::Message message;

  ASSERT_EQ(message.encode(other.attachment()), true);

  rl::core::Messages messages;
  messages.emplace_back(std::move(message));
  ASSERT_EQ(chan.sendMessages(std::move(messages)),
            rl::core::IOResult::Success);

  thread.join();
}

TEST(ChannelTest, AliasingChannels) {
  /*
   *  Create two channels and pass the handle of one as a message. Read that
   *  message on a separate runloop and create an alias of the original channel
   *  from just the message attachment handle. Write on that aliased channel and
   *  verify the same message was read on the source of the alias.
   */
  rl::core::Channel chan;

  rl::core::Latch aliasLatch(1);
  chan.setMessageCallback(
      [&](rl::core::Message message, rl::core::Namespace* ns) {

        rl::core::RawAttachment attachment;

        ASSERT_EQ(message.decode(attachment), true);

        /*
         *  There is only one attachment.
         */
        ASSERT_EQ(message.decode(attachment), false);

        rl::core::EventLoop::Current()->terminate();

        /*
         *  Alias of the channel
         */
        rl::core::Channel aliasOther(std::move(attachment));
        ASSERT_NE(aliasOther.source(), nullptr);

        rl::core::Message messageToAlias;
        char x = 'x';
        ASSERT_EQ(messageToAlias.encode(x), true);

        rl::core::Messages messages;
        messages.emplace_back(std::move(messageToAlias));

        ASSERT_EQ(aliasOther.sendMessages(std::move(messages)),
                  rl::core::IOResult::Success);
        aliasLatch.countDown();
      });

  rl::core::Latch latch(1);
  std::thread thread([&]() {
    auto loop = rl::core::EventLoop::Current();
    loop->addSource(chan.source());

    loop->loop([&]() { latch.countDown(); });
  });

  latch.wait();

  /*
   *  Source channel
   */
  rl::core::Channel other;

  rl::core::Message message;
  ASSERT_EQ(message.encode(other.attachment()), true);
  rl::core::Messages messages;
  messages.emplace_back(std::move(message));
  ASSERT_EQ(chan.sendMessages(std::move(messages)),
            rl::core::IOResult::Success);

  bool otherMessageRead = false;
  other.setMessageCallback(
      [&](rl::core::Message message, rl::core::Namespace* ns) {
        otherMessageRead = true;

        char someChar = 'a';
        ASSERT_EQ(message.decode(someChar, nullptr), true);
        ASSERT_EQ(someChar, 'x');
      });

  aliasLatch.wait();
  ASSERT_EQ(other.readPendingMessageNow(), rl::core::IOResult::Success);

  ASSERT_EQ(otherMessageRead, true);

  thread.join();
}

TEST(ChannelTest, SendAttachmentAndDataOverChannels) {
  rl::core::Channel chan;

  chan.setMessageCallback(
      [&](rl::core::Message message, rl::core::Namespace* ns) {

        rl::core::RawAttachment attachment;

        ASSERT_EQ(message.decode(attachment), true);

        rl::core::Channel consumer(std::move(attachment));

        /*
         *  Only one attachment is present.
         */
        ASSERT_EQ(message.decode(attachment), false);

        auto character = 'a';
        ASSERT_EQ(message.decode(character, nullptr), true);
        ASSERT_EQ(character, 'x');
        rl::core::EventLoop::Current()->terminate();
      });

  rl::core::Latch latch(1);
  std::thread thread([&]() {
    auto loop = rl::core::EventLoop::Current();
    loop->addSource(chan.source());

    loop->loop([&]() { latch.countDown(); });
  });

  latch.wait();

  rl::core::Channel other;

  rl::core::Message message;
  ASSERT_EQ(message.encode(other.attachment()), true);

  auto someChar = 'x';
  ASSERT_EQ(message.encode(someChar), true);

  rl::core::Messages messages;
  messages.emplace_back(std::move(message));
  ASSERT_EQ(chan.sendMessages(std::move(messages)),
            rl::core::IOResult::Success);

  thread.join();
}

TEST(ChannelTest, SendMultipleAttachmentsAndDataOverChannels) {
  rl::core::Channel chan;

  chan.setMessageCallback(
      [&](rl::core::Message message, rl::core::Namespace* ns) {
        size_t count = 0;

        while (true) {
          rl::core::RawAttachment attachment;
          if (message.decode(attachment)) {
            rl::core::Channel consumer(std::move(attachment));
            count++;
          } else {
            break;
          }
        }
        ASSERT_EQ(count, 7);

        auto character = 'a';
        ASSERT_EQ(message.decode(character, nullptr), true);
        ASSERT_EQ(character, 'x');
        ASSERT_EQ(message.readCompleted(), true);
        rl::core::EventLoop::Current()->terminate();
      });

  rl::core::Latch latch(1);
  std::thread thread([&]() {
    auto loop = rl::core::EventLoop::Current();
    loop->addSource(chan.source());

    loop->loop([&]() { latch.countDown(); });
  });

  latch.wait();

  rl::core::Channel other0;
  rl::core::Channel other1;
  rl::core::Channel other2;
  rl::core::Channel other3;
  rl::core::Channel other4;
  rl::core::Channel other5;
  rl::core::Channel other6;

  rl::core::Message message;

  ASSERT_EQ(message.encode(other0.attachment()), true);
  ASSERT_EQ(message.encode(other1.attachment()), true);
  ASSERT_EQ(message.encode(other2.attachment()), true);
  ASSERT_EQ(message.encode(other3.attachment()), true);
  ASSERT_EQ(message.encode(other4.attachment()), true);
  ASSERT_EQ(message.encode(other5.attachment()), true);
  ASSERT_EQ(message.encode(other6.attachment()), true);

  auto someChar = 'x';
  ASSERT_EQ(message.encode(someChar), true);

  rl::core::Messages messages;
  messages.emplace_back(std::move(message));
  ASSERT_EQ(chan.sendMessages(std::move(messages)),
            rl::core::IOResult::Success);

  thread.join();
}

/**
 *  This is to test the passing of an OOL buffer along with attachments. Only
 *  a concern on Socket channels
 */
TEST_SLOW(ChannelTest, TestLargeReadWriteWithAttachments) {
  const auto rawSize = 20000009;

  rl::core::Channel channel;

  rl::core::Latch latch(1);

  size_t sizeWritten = 0;

  std::thread thread([&] {
    auto loop = rl::core::EventLoop::Current();
    ASSERT_TRUE(loop != nullptr);

    auto source = channel.source();
    ASSERT_TRUE(loop->addSource(source));

    channel.setMessageCallback(
        [&](rl::core::Message message, rl::core::Namespace* ns) {
          ASSERT_TRUE(message.size() == rawSize);
          ASSERT_TRUE(MemorySetOrCheckPattern(message.data(), rawSize,
                                              false /* check */));

          size_t count = 0;

          while (true) {
            rl::core::RawAttachment attachment;
            if (message.decode(attachment)) {
              rl::core::Channel consumer(std::move(attachment));
              count++;
            } else {
              break;
            }
          }

          ASSERT_EQ(count, 7);

          ASSERT_TRUE(loop == rl::core::EventLoop::Current());
          loop->terminate();
        });

    loop->loop([&] { latch.countDown(); });
  });

  latch.wait();

  rl::core::Messages messages;

  /*
   *  Write a large message (enough to trip the OOL check)
   */
  rl::core::Message message;

  ASSERT_TRUE(MemorySetOrCheckPattern(message.encodeRaw<uint8_t>(rawSize),
                                      rawSize, true /* set */));

  sizeWritten = message.size();
  ASSERT_EQ(sizeWritten, rawSize);

  /*
   *  Set a few attachments
   */
  rl::core::Channel other0;
  rl::core::Channel other1;
  rl::core::Channel other2;
  rl::core::Channel other3;
  rl::core::Channel other4;
  rl::core::Channel other5;
  rl::core::Channel other6;

  ASSERT_EQ(message.encode(other0.attachment()), true);
  ASSERT_EQ(message.encode(other1.attachment()), true);
  ASSERT_EQ(message.encode(other2.attachment()), true);
  ASSERT_EQ(message.encode(other3.attachment()), true);
  ASSERT_EQ(message.encode(other4.attachment()), true);
  ASSERT_EQ(message.encode(other5.attachment()), true);
  ASSERT_EQ(message.encode(other6.attachment()), true);

  messages.emplace_back(std::move(message));
  ASSERT_EQ(channel.sendMessages(std::move(messages)),
            rl::core::IOResult::Success);

  thread.join();
}

/**
 *  This is to ensure that inline memory buffers dont mess up the attachments
 *  they are sent with. Only a concern on Socket channels
 */
TEST(ChannelTest, TestSmallReadWriteWithAttachments) {
  const auto rawSize = 27;

  rl::core::Channel channel;

  rl::core::Latch latch(1);

  size_t sizeWritten = 0;

  std::thread thread([&] {
    auto loop = rl::core::EventLoop::Current();
    ASSERT_TRUE(loop != nullptr);

    auto source = channel.source();
    ASSERT_TRUE(loop->addSource(source));

    channel.setMessageCallback(
        [&](rl::core::Message message, rl::core::Namespace* ns) {
          ASSERT_TRUE(message.size() == rawSize);
          ASSERT_TRUE(MemorySetOrCheckPattern(message.data(), rawSize,
                                              false /* check */));

          size_t count = 0;

          while (true) {
            rl::core::RawAttachment attachment;
            if (message.decode(attachment)) {
              rl::core::Channel consumer(std::move(attachment));
              count++;
            } else {
              break;
            }
          }

          ASSERT_EQ(count, 3);

          ASSERT_TRUE(loop == rl::core::EventLoop::Current());
          loop->terminate();
        });

    loop->loop([&] { latch.countDown(); });
  });

  latch.wait();

  rl::core::Messages messages;

  /*
   *  Write a large message (enough to trip the OOL check)
   */
  rl::core::Message message;

  ASSERT_TRUE(MemorySetOrCheckPattern(message.encodeRaw<uint8_t>(rawSize),
                                      rawSize, true /* set */));

  sizeWritten = message.size();
  ASSERT_EQ(sizeWritten, rawSize);

  /*
   *  Set a few attachments
   */
  rl::core::Channel other0;
  rl::core::Channel other1;
  rl::core::Channel other2;

  ASSERT_EQ(message.encode(other0.attachment()), true);
  ASSERT_EQ(message.encode(other1.attachment()), true);
  ASSERT_EQ(message.encode(other2.attachment()), true);

  messages.emplace_back(std::move(message));
  ASSERT_EQ(channel.sendMessages(std::move(messages)),
            rl::core::IOResult::Success);

  thread.join();
}
