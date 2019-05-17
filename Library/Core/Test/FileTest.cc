/*
 *  This source file is part of the Radar project.
 *  Licensed under the MIT License. See LICENSE file for details.
 */

#include <Core/Channel.h>
#include <Core/File.h>
#include <TestRunner/TestRunner.h>
#include <thread>

namespace rl {
namespace core {
namespace testing {

TEST(FileTest, SimpleFail) {
  File file(std::string{"file://no/such/file/exists.txt"});
  ASSERT_FALSE(file.isValid());
}

TEST(FileTest, SimpleSuccess) {
  File file(std::string{"file://Hello.txt"});
  ASSERT_TRUE(file.isValid());
}

TEST(FileTest, TestFileSendsOverChannel) {
  Channel channel;

  std::thread thread([&]() {
    auto loop = EventLoop::Current();

    auto source = channel.source();

    channel.setMessageCallback([&](Message message, Namespace*) {
      RawAttachment attachment;

      ASSERT_TRUE(message.decode(attachment));

      FileHandle fileHandle(std::move(attachment));

      ASSERT_TRUE(fileHandle.isValid());

      FileMapping mapping(fileHandle);

      ASSERT_EQ(mapping.size(), 13u);

      EventLoop::Current()->terminate();
    });

    ASSERT_TRUE(loop->addSource(source));

    loop->loop();
  });

  auto file = std::make_shared<FileHandle>(std::string{"file://Hello.txt"});

  ASSERT_TRUE(file->isValid());

  Messages messages;

  Message message;

  ASSERT_TRUE(message.encode(std::move(file)));

  messages.emplace_back(std::move(message));

  ASSERT_EQ(channel.sendMessages(std::move(messages)), IOResult::Success);

  thread.join();
}

}  // namespace testing
}  // namespace core
}  // namespace rl
