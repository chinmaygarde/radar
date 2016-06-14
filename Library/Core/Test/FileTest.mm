// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <TestRunner/TestRunner.h>

#include <Core/File.h>
#include <Core/Channel.h>
#include <thread>

RL_DECLARE_TEST_START(FileTest)

TEST(FileTest, SimpleFail) {
  rl::core::File file(std::string{"file://no/such/file/exists.txt"});
  ASSERT_FALSE(file.isValid());
}

TEST(FileTest, SimpleSuccess) {
  rl::core::File file(std::string{"file://Hello.txt"});
  ASSERT_TRUE(file.isValid());
}

TEST(FileTest, TestFileSendsOverChannel) {
  rl::core::Channel channel;

  std::thread thread([&]() {
    auto loop = rl::core::EventLoop::Current();

    auto source = channel.source();

    channel.setMessageCallback(
        [&](rl::core::Message message, rl::core::Namespace*) {
          rl::core::RawAttachment attachment;

          ASSERT_TRUE(message.decode(attachment));

          rl::core::FileHandle fileHandle(std::move(attachment));

          ASSERT_TRUE(fileHandle.isValid());

          rl::core::EventLoop::Current()->terminate();
        });

    ASSERT_TRUE(loop->addSource(source));

    loop->loop();
  });

  auto file =
      std::make_shared<rl::core::FileHandle>(std::string{"file://Hello.txt"});

  ASSERT_TRUE(file->isValid());

  rl::core::Messages messages;

  rl::core::Message message;

  ASSERT_TRUE(message.encode(std::move(file)));

  messages.emplace_back(std::move(message));

  ASSERT_EQ(channel.sendMessages(std::move(messages)),
            rl::core::IOResult::Success);

  thread.join();
}

RL_DECLARE_TEST_END
