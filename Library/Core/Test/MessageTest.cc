// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <Core/Channel.h>
#include <Core/Message.h>
#include <TestRunner/TestRunner.h>

TEST(MessageTest, SimpleEncode) {
  rl::core::Message message;

  ASSERT_TRUE(message.encode(30.0));

  ASSERT_TRUE(message.size() == sizeof(double));
}

TEST(MessageTest, MultipleEncode) {
  rl::core::Message message;

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
  rl::core::Message message;

  uint32_t val = 40;

  ASSERT_TRUE(message.encode(val));

  val = 222;

  rl::core::Message decoder(message.data(), message.size(), false);

  ASSERT_TRUE(decoder.decode(val, nullptr));
  ASSERT_TRUE(val == 40);
}

TEST(MessageTest, MutltipleDecode) {
  rl::core::Message message;

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

  rl::core::Message decoder(message.data(), message.size(), false);

  ASSERT_TRUE(decoder.decode(val, nullptr));
  ASSERT_TRUE(decoder.decode(foo, nullptr));
  ASSERT_TRUE(decoder.decode(a, nullptr));
  ASSERT_TRUE(decoder.decode(b, nullptr));
  ASSERT_TRUE(decoder.decode(c, nullptr));

  ASSERT_FALSE(decoder.decode(val, nullptr));

  ASSERT_TRUE(val == 40);
  ASSERT_TRUE(foo == true);
  ASSERT_TRUE(a == 3.0f);
  ASSERT_TRUE(b == 20.0);
  ASSERT_TRUE(c == 'c');
}

TEST(MessageTest, EncodeAttachment) {
  rl::core::Channel channel;

  rl::core::Message message;

  ASSERT_EQ(message.encode(channel.attachment()), true);
}

TEST(MessageTest, EncodeMultipleValidAttachment) {
  rl::core::Message message;

  for (size_t i = 0; i < 10; i++) {
    rl::core::Channel channel;
    ASSERT_EQ(message.encode(channel.attachment()), true);
  }
}

TEST(MessageTest, EncodeDecodeMultipleValidAttachment) {
  rl::core::Message message;

  const size_t count = 16;

  std::vector<std::unique_ptr<rl::core::Channel>> channels;

  for (size_t i = 0; i < count; i++) {
    channels.emplace_back(std::make_unique<rl::core::Channel>());
  }

  for (const auto& channel : channels) {
    ASSERT_EQ(message.encode(channel->attachment()), true);
  }

  rl::core::Channel pipe;

  rl::core::Messages messages;

  messages.emplace_back(std::move(message));

  ASSERT_EQ(pipe.sendMessages(std::move(messages)),
            rl::core::IOResult::Success);

  auto messagesRead = pipe.drainPendingMessages(rl::core::ClockDurationNano{0});

  ASSERT_EQ(messagesRead.size(), 1);

  auto& messageRead = messagesRead[0];

  size_t initialRead = 0;

  for (size_t i = 0; i < count / 2; i++) {
    rl::core::RawAttachment attachment;
    ASSERT_EQ(messageRead.decode(attachment), true);
    rl::core::Channel consumer(std::move(attachment));
    initialRead++;
  }

  size_t secondaryRead = 0;
  while (true) {
    rl::core::RawAttachment attachment;
    if (messageRead.decode(attachment)) {
      rl::core::Channel consumer(std::move(attachment));
      secondaryRead++;
      continue;
    }
    break;
  }

  ASSERT_EQ(initialRead, secondaryRead);
}
