/*
 *  This source file is part of the Radar project.
 *  Licensed under the MIT License. See LICENSE file for details.
 */

#include <thread>

#include <Core/Channel.h>
#include <Core/Latch.h>
#include <Core/Stopwatch.h>
#include <TestRunner/TestRunner.h>

namespace rl {
namespace core {
namespace testing {

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
  Channel channel;
  ASSERT_TRUE(channel.source() != nullptr);
}

TEST_SLOW(ChannelTest, ReadTimeout) {
  auto ms = 50;

  Channel channel;
  rl::instrumentation::Stopwatch stopwatch;

  {
    rl::instrumentation::AutoStopwatchLap lap(stopwatch);
    auto result = channel.readPendingMessageNow(ClockDurationMilli(ms));
    ASSERT_EQ(result, IOResult::Timeout);
  }

  ASSERT_GE(stopwatch.lastLap(), ClockDurationMilli(ms));
}

#if 0

TEST(ChannelTest, ChannelDeathRemovesChannelAliasesFromWaitsets) {
  Channel channel;
  Message::Attachment channelAttachment =
      channel.asMessageAttachment();

  bool aliasTerminated = false;
  Latch readyLatch(1);
  std::thread waiter([&]() {
    thread::SetName("waiter");
    auto loop = EventLoop::Current();

    Channel aliasChannel(channelAttachment);

    aliasChannel.setTerminationCallback([&](Channel&) {
      aliasTerminated = true;
      ASSERT_EQ(loop, EventLoop::Current());
      EventLoop::Current()->terminate();
    });

    loop->addSource(aliasChannel.source());

    loop->loop([&]() { readyLatch.countDown(); });
  });

  readyLatch.wait();

  bool sourceTerminated = false;
  channel.setTerminationCallback(
      [&](Channel&) { sourceTerminated = true; });

  channel.terminate();

  waiter.join();

  ASSERT_EQ(sourceTerminated, true);
  ASSERT_EQ(aliasTerminated, true);
}

#endif

TEST(ChannelTest, TestSimpleReads) {
  Channel channel;

  Latch latch(1);

  std::thread thread([&] {
    auto loop = EventLoop::Current();
    ASSERT_TRUE(loop != nullptr);

    auto source = channel.source();
    ASSERT_TRUE(loop->addSource(source));

    channel.setMessageCallback([&](Message m, Namespace* ns) {
      ASSERT_TRUE(loop == EventLoop::Current());
      loop->terminate();
    });

    loop->loop([&] { latch.countDown(); });
  });

  latch.wait();

  Messages messages;
  Message m;

  char c = 'c';
  ASSERT_TRUE(m.encode(c));
  messages.emplace_back(std::move(m));
  ASSERT_EQ(channel.sendMessages(std::move(messages)), IOResult::Success);

  thread.join();
}

TEST_SLOW(ChannelTest, TestLargeReadWrite) {
  Channel channel;

  Latch latch(1);

  const size_t rawSize = 50000003;

  std::thread thread([&] {
    auto loop = EventLoop::Current();
    ASSERT_TRUE(loop != nullptr);

    auto source = channel.source();
    ASSERT_TRUE(loop->addSource(source));

    channel.setMessageCallback([&](Message m, Namespace* ns) {
      ASSERT_TRUE(m.size() == rawSize);
      ASSERT_TRUE(
          MemorySetOrCheckPattern(m.data(), rawSize, false /* check */));
      ASSERT_TRUE(loop == EventLoop::Current());
      loop->terminate();
    });

    loop->loop([&] { latch.countDown(); });
  });

  latch.wait();

  Messages messages;
  Message m;

  ASSERT_TRUE(MemorySetOrCheckPattern(m.encodeRaw<uint8_t>(rawSize), rawSize,
                                      true /* set */));
  size_t sizeWritten = m.size();
  ASSERT_EQ(sizeWritten, rawSize);

  messages.emplace_back(std::move(m));
  ASSERT_EQ(channel.sendMessages(std::move(messages)), IOResult::Success);

  thread.join();
}

TEST(ChannelTest, TestSimpleReadContents) {
  Channel channel;

  Latch latch(1);

  std::thread thread([&] {
    auto loop = EventLoop::Current();
    ASSERT_TRUE(loop != nullptr);

    auto source = channel.source();
    ASSERT_TRUE(loop->addSource(source));

    channel.setMessageCallback([&](Message m, Namespace* ns) {
      ASSERT_TRUE(loop == EventLoop::Current());

      char c = 'a';
      int d = 22;
      ASSERT_TRUE(m.decode(c, nullptr));
      ASSERT_TRUE(m.decode(d, nullptr));
      ASSERT_EQ(m.size(), m.sizeRead());
      ASSERT_EQ(c, 'c');
      ASSERT_EQ(d, 222);

      loop->terminate();
    });

    loop->loop([&] { latch.countDown(); });
  });

  latch.wait();

  Messages messages;
  Message m;

  char c = 'c';
  int d = 222;
  ASSERT_TRUE(m.encode(c));
  ASSERT_TRUE(m.encode(d));

  messages.emplace_back(std::move(m));
  ASSERT_EQ(channel.sendMessages(std::move(messages)), IOResult::Success);

  thread.join();
}

TEST(ChannelTest, ZeroTimeoutReadsDontHang) {
  Channel channel;
  Messages messages = channel.drainPendingMessages(ClockDurationNano{0});
  ASSERT_EQ(messages.size(), 0u);
}

TEST(ChannelTest, SendAttachmentsOverChannels) {
  Channel chan;

  chan.setMessageCallback([&](Message message, Namespace* ns) {
    RawAttachment attachment;

    ASSERT_EQ(message.decode(attachment), true);

    Channel consumer(std::move(attachment));

    /*
     *  There is only one attachment
     */
    ASSERT_EQ(message.decode(attachment), false);

    ASSERT_EQ(message.readCompleted(), true);

    EventLoop::Current()->terminate();
  });

  Latch latch(1);
  std::thread thread([&]() {
    auto loop = EventLoop::Current();
    loop->addSource(chan.source());

    loop->loop([&]() { latch.countDown(); });
  });

  latch.wait();

  Channel other;

  Message message;

  ASSERT_EQ(message.encode(other.attachment()), true);

  Messages messages;
  messages.emplace_back(std::move(message));
  ASSERT_EQ(chan.sendMessages(std::move(messages)), IOResult::Success);

  thread.join();
}

TEST(ChannelTest, AliasingChannels) {
  /*
   *  Create two channels and pass the handle of one as a message. Read that
   *  message on a separate runloop and create an alias of the original channel
   *  from just the message attachment handle. Write on that aliased channel and
   *  verify the same message was read on the source of the alias.
   */
  Channel chan;

  Latch aliasLatch(1);
  chan.setMessageCallback([&](Message message, Namespace* ns) {
    RawAttachment attachment;

    ASSERT_EQ(message.decode(attachment), true);

    /*
     *  There is only one attachment.
     */
    ASSERT_EQ(message.decode(attachment), false);

    EventLoop::Current()->terminate();

    /*
     *  Alias of the channel
     */
    Channel aliasOther(std::move(attachment));
    ASSERT_NE(aliasOther.source(), nullptr);

    Message messageToAlias;
    char x = 'x';
    ASSERT_EQ(messageToAlias.encode(x), true);

    Messages messages;
    messages.emplace_back(std::move(messageToAlias));

    ASSERT_EQ(aliasOther.sendMessages(std::move(messages)), IOResult::Success);
    aliasLatch.countDown();
  });

  Latch latch(1);
  std::thread thread([&]() {
    auto loop = EventLoop::Current();
    loop->addSource(chan.source());

    loop->loop([&]() { latch.countDown(); });
  });

  latch.wait();

  /*
   *  Source channel
   */
  Channel other;

  Message message;
  ASSERT_EQ(message.encode(other.attachment()), true);
  Messages messages;
  messages.emplace_back(std::move(message));
  ASSERT_EQ(chan.sendMessages(std::move(messages)), IOResult::Success);

  bool otherMessageRead = false;
  other.setMessageCallback([&](Message message, Namespace* ns) {
    otherMessageRead = true;

    char someChar = 'a';
    ASSERT_EQ(message.decode(someChar, nullptr), true);
    ASSERT_EQ(someChar, 'x');
  });

  aliasLatch.wait();
  ASSERT_EQ(other.readPendingMessageNow(), IOResult::Success);

  ASSERT_EQ(otherMessageRead, true);

  thread.join();
}

TEST(ChannelTest, SendAttachmentAndDataOverChannels) {
  Channel chan;

  chan.setMessageCallback([&](Message message, Namespace* ns) {
    RawAttachment attachment;

    ASSERT_EQ(message.decode(attachment), true);

    Channel consumer(std::move(attachment));

    /*
     *  Only one attachment is present.
     */
    ASSERT_EQ(message.decode(attachment), false);

    auto character = 'a';
    ASSERT_EQ(message.decode(character, nullptr), true);
    ASSERT_EQ(character, 'x');
    EventLoop::Current()->terminate();
  });

  Latch latch(1);
  std::thread thread([&]() {
    auto loop = EventLoop::Current();
    loop->addSource(chan.source());

    loop->loop([&]() { latch.countDown(); });
  });

  latch.wait();

  Channel other;

  Message message;
  ASSERT_EQ(message.encode(other.attachment()), true);

  auto someChar = 'x';
  ASSERT_EQ(message.encode(someChar), true);

  Messages messages;
  messages.emplace_back(std::move(message));
  ASSERT_EQ(chan.sendMessages(std::move(messages)), IOResult::Success);

  thread.join();
}

TEST(ChannelTest, SendMultipleAttachmentsAndDataOverChannels) {
  Channel chan;

  chan.setMessageCallback([&](Message message, Namespace* ns) {
    size_t count = 0;

    while (true) {
      RawAttachment attachment;
      if (message.decode(attachment)) {
        Channel consumer(std::move(attachment));
        count++;
      } else {
        break;
      }
    }
    ASSERT_EQ(count, 7u);

    auto character = 'a';
    ASSERT_EQ(message.decode(character, nullptr), true);
    ASSERT_EQ(character, 'x');
    ASSERT_EQ(message.readCompleted(), true);
    EventLoop::Current()->terminate();
  });

  Latch latch(1);
  std::thread thread([&]() {
    auto loop = EventLoop::Current();
    loop->addSource(chan.source());

    loop->loop([&]() { latch.countDown(); });
  });

  latch.wait();

  Channel other0;
  Channel other1;
  Channel other2;
  Channel other3;
  Channel other4;
  Channel other5;
  Channel other6;

  Message message;

  ASSERT_EQ(message.encode(other0.attachment()), true);
  ASSERT_EQ(message.encode(other1.attachment()), true);
  ASSERT_EQ(message.encode(other2.attachment()), true);
  ASSERT_EQ(message.encode(other3.attachment()), true);
  ASSERT_EQ(message.encode(other4.attachment()), true);
  ASSERT_EQ(message.encode(other5.attachment()), true);
  ASSERT_EQ(message.encode(other6.attachment()), true);

  auto someChar = 'x';
  ASSERT_EQ(message.encode(someChar), true);

  Messages messages;
  messages.emplace_back(std::move(message));
  ASSERT_EQ(chan.sendMessages(std::move(messages)), IOResult::Success);

  thread.join();
}

/**
 *  This is to test the passing of an OOL buffer along with attachments. Only
 *  a concern on Socket channels
 */
TEST_SLOW(ChannelTest, TestLargeReadWriteWithAttachments) {
  const size_t rawSize = 20000009;

  Channel channel;

  Latch latch(1);

  size_t sizeWritten = 0;

  std::thread thread([&] {
    auto loop = EventLoop::Current();
    ASSERT_TRUE(loop != nullptr);

    auto source = channel.source();
    ASSERT_TRUE(loop->addSource(source));

    channel.setMessageCallback([&](Message message, Namespace* ns) {
      ASSERT_TRUE(message.size() == rawSize);
      ASSERT_TRUE(
          MemorySetOrCheckPattern(message.data(), rawSize, false /* check */));

      size_t count = 0;

      while (true) {
        RawAttachment attachment;
        if (message.decode(attachment)) {
          Channel consumer(std::move(attachment));
          count++;
        } else {
          break;
        }
      }

      ASSERT_EQ(count, 7u);

      ASSERT_TRUE(loop == EventLoop::Current());
      loop->terminate();
    });

    loop->loop([&] { latch.countDown(); });
  });

  latch.wait();

  Messages messages;

  /*
   *  Write a large message (enough to trip the OOL check)
   */
  Message message;

  ASSERT_TRUE(MemorySetOrCheckPattern(message.encodeRaw<uint8_t>(rawSize),
                                      rawSize, true /* set */));

  sizeWritten = message.size();
  ASSERT_EQ(sizeWritten, rawSize);

  /*
   *  Set a few attachments
   */
  Channel other0;
  Channel other1;
  Channel other2;
  Channel other3;
  Channel other4;
  Channel other5;
  Channel other6;

  ASSERT_EQ(message.encode(other0.attachment()), true);
  ASSERT_EQ(message.encode(other1.attachment()), true);
  ASSERT_EQ(message.encode(other2.attachment()), true);
  ASSERT_EQ(message.encode(other3.attachment()), true);
  ASSERT_EQ(message.encode(other4.attachment()), true);
  ASSERT_EQ(message.encode(other5.attachment()), true);
  ASSERT_EQ(message.encode(other6.attachment()), true);

  messages.emplace_back(std::move(message));
  ASSERT_EQ(channel.sendMessages(std::move(messages)), IOResult::Success);

  thread.join();
}

/**
 *  This is to ensure that inline memory buffers dont mess up the attachments
 *  they are sent with. Only a concern on Socket channels
 */
TEST(ChannelTest, TestSmallReadWriteWithAttachments) {
  const size_t rawSize = 27;

  Channel channel;

  Latch latch(1);

  size_t sizeWritten = 0;

  std::thread thread([&] {
    auto loop = EventLoop::Current();
    ASSERT_TRUE(loop != nullptr);

    auto source = channel.source();
    ASSERT_TRUE(loop->addSource(source));

    channel.setMessageCallback([&](Message message, Namespace* ns) {
      ASSERT_TRUE(message.size() == rawSize);
      ASSERT_TRUE(
          MemorySetOrCheckPattern(message.data(), rawSize, false /* check */));

      size_t count = 0;

      while (true) {
        RawAttachment attachment;
        if (message.decode(attachment)) {
          Channel consumer(std::move(attachment));
          count++;
        } else {
          break;
        }
      }

      ASSERT_EQ(count, 3u);

      ASSERT_TRUE(loop == EventLoop::Current());
      loop->terminate();
    });

    loop->loop([&] { latch.countDown(); });
  });

  latch.wait();

  Messages messages;

  /*
   *  Write a large message (enough to trip the OOL check)
   */
  Message message;

  ASSERT_TRUE(MemorySetOrCheckPattern(message.encodeRaw<uint8_t>(rawSize),
                                      rawSize, true /* set */));

  sizeWritten = message.size();
  ASSERT_EQ(sizeWritten, rawSize);

  /*
   *  Set a few attachments
   */
  Channel other0;
  Channel other1;
  Channel other2;

  ASSERT_EQ(message.encode(other0.attachment()), true);
  ASSERT_EQ(message.encode(other1.attachment()), true);
  ASSERT_EQ(message.encode(other2.attachment()), true);

  messages.emplace_back(std::move(message));
  ASSERT_EQ(channel.sendMessages(std::move(messages)), IOResult::Success);

  thread.join();
}

}  // namespace testing
}  // namespace core
}  // namespace rl
