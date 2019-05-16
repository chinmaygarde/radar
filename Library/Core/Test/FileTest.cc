/*
 *  This source file is part of the Radar project.
 *  Licensed under the MIT License. See LICENSE file for details.
 */

#include <Core/Channel.h>
#include <Core/File.h>
#include <TestRunner/TestRunner.h>
#include <thread>

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

          rl::core::FileMapping mapping(fileHandle);

          ASSERT_EQ(mapping.size(), 13u);

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
