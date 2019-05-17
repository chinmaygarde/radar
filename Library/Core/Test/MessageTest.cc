/*
 *  This source file is part of the Radar project.
 *  Licensed under the MIT License. See LICENSE file for details.
 */

#include <Core/Channel.h>
#include <Core/Message.h>
#include <TestRunner/TestRunner.h>

namespace rl {
namespace core {
namespace testing {

TEST(MessageTest, SimpleEncode) {
  Message message;

  ASSERT_TRUE(message.encode(30.0));

  ASSERT_TRUE(message.size() == sizeof(double));
}

TEST(MessageTest, MultipleEncode) {
  Message message;

  size_t s = 0;
  ASSERT_TRUE(message.encode(30.0));
  ASSERT_TRUE(message.size() - sizeof(double) == s);
  s += sizeof(double);

  ASSERT_TRUE(message.encode(true));
  ASSERT_TRUE(message.size() - sizeof(bool) == s);
  s += sizeof(bool);

  ASSERT_TRUE(message.encode('c'));
  ASSERT_TRUE(message.size() - sizeof(char) == s);
  s += sizeof(char);

  ASSERT_TRUE(message.encode(50.0f));
  ASSERT_TRUE(message.size() - sizeof(float) == s);
}

TEST(MessageTest, SimpleDecode) {
  Message message;

  uint32_t val = 40;

  ASSERT_TRUE(message.encode(val));

  val = 222;

  Message decoder(message.data(), message.size(), false);

  ASSERT_TRUE(decoder.decode(val, nullptr));
  ASSERT_EQ(val, 40u);
}

TEST(MessageTest, MutltipleDecode) {
  Message message;

  uint32_t val = 40;
  bool foo = true;
  float a = 3.0f;
  double b = 20.0;
  char c = 'c';

  ASSERT_TRUE(message.encode(val));
  ASSERT_TRUE(message.encode(foo));
  ASSERT_TRUE(message.encode(a));
  ASSERT_TRUE(message.encode(b));
  ASSERT_TRUE(message.encode(c));

  val = 0;
  foo = false;
  a = 0.0f;
  b = 0.0;
  c = 'd';

  Message decoder(message.data(), message.size(), false);

  ASSERT_TRUE(decoder.decode(val, nullptr));
  ASSERT_TRUE(decoder.decode(foo, nullptr));
  ASSERT_TRUE(decoder.decode(a, nullptr));
  ASSERT_TRUE(decoder.decode(b, nullptr));
  ASSERT_TRUE(decoder.decode(c, nullptr));

  ASSERT_FALSE(decoder.decode(val, nullptr));

  ASSERT_EQ(val, 40u);
  ASSERT_TRUE(foo);
  ASSERT_EQ(a, 3.0f);
  ASSERT_EQ(b, 20.0);
  ASSERT_EQ(c, 'c');
}

TEST(MessageTest, EncodeAttachment) {
  Channel channel;

  Message message;

  ASSERT_EQ(message.encode(channel.attachment()), true);
}

TEST(MessageTest, EncodeMultipleValidAttachment) {
  Message message;

  for (size_t i = 0; i < 10; i++) {
    Channel channel;
    ASSERT_EQ(message.encode(channel.attachment()), true);
  }
}

TEST(MessageTest, EncodeDecodeMultipleValidAttachment) {
  Message message;

  const size_t count = 16;

  std::vector<std::unique_ptr<Channel>> channels;

  for (size_t i = 0; i < count; i++) {
    channels.emplace_back(std::make_unique<Channel>());
  }

  for (const auto& channel : channels) {
    ASSERT_EQ(message.encode(channel->attachment()), true);
  }

  Channel pipe;

  Messages messages;

  messages.emplace_back(std::move(message));

  ASSERT_EQ(pipe.sendMessages(std::move(messages)), IOResult::Success);

  auto messagesRead = pipe.drainPendingMessages(ClockDurationNano{0});

  ASSERT_EQ(messagesRead.size(), 1u);

  auto& messageRead = messagesRead[0];

  size_t initialRead = 0;

  for (size_t i = 0; i < count / 2; i++) {
    RawAttachment attachment;
    ASSERT_EQ(messageRead.decode(attachment), true);
    Channel consumer(std::move(attachment));
    initialRead++;
  }

  size_t secondaryRead = 0;
  while (true) {
    RawAttachment attachment;
    if (messageRead.decode(attachment)) {
      Channel consumer(std::move(attachment));
      secondaryRead++;
      continue;
    }
    break;
  }

  ASSERT_EQ(initialRead, secondaryRead);
}

}  // namespace testing
}  // namespace core
}  // namespace rl
