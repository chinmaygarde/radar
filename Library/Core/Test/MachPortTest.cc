/*
 *  This source file is part of the Radar project.
 *  Licensed under the MIT License. See LICENSE file for details.
 */

#include <TestRunner/TestRunner.h>

#include "MachPort.h"

namespace rl {
namespace core {
namespace testing {

#if RL_CHANNELS == RL_CHANNELS_MACH

TEST(MachPortTest, CanCreatePorts) {
  {
    MachPort port(MachPort::Type::None);
    ASSERT_FALSE(port.isValid());
  }

  {
    MachPort port(MachPort::Type::Send);
    ASSERT_FALSE(port.isValid());
    ASSERT_EQ(port.type(), MachPort::Type::None);
  }

  {
    MachPort port(MachPort::Type::SendReceive);
    ASSERT_TRUE(port.isValid());
    ASSERT_EQ(port.type(), MachPort::Type::SendReceive);
  }

  {
    MachPort port(MachPort::Type::PortSet);
    ASSERT_TRUE(port.isValid());
    ASSERT_EQ(port.type(), MachPort::Type::PortSet);
  }
}

TEST(MachPortTest, CanSendMessage) {
  MachPort port(MachPort::Type::SendReceive);
  ASSERT_TRUE(port.isValid());

  {
    Message message;
    char c = 'c';
    ASSERT_TRUE(message.encode(c));
    Messages messages;
    messages.emplace_back(std::move(message));

    ASSERT_EQ(port.sendMessages(std::move(messages), ClockDurationMilli{10}),
              IOResult::Success);
  }

  {
    auto read_result = port.receiveMessage(ClockDurationMilli{10});
    ASSERT_EQ(read_result.first, IOResult::Success);
    char c = '\0';
    ASSERT_TRUE(read_result.second.decode(c));
    ASSERT_EQ(c, 'c');
    ASSERT_FALSE(read_result.second.decode(c));
  }
}

TEST(MachPortTest, CannotSendMessagesOnPortSets) {
  MachPort portSet(MachPort::Type::PortSet);
  Message message;
  char c = 'c';
  ASSERT_TRUE(message.encode(c));
  Messages messages;
  messages.emplace_back(std::move(message));

  // Fails with "invalid destination port".
  ASSERT_EQ(portSet.sendMessages(std::move(messages), ClockDurationMilli{10}),
            IOResult::Failure);
}

TEST(MachPortTest, CanInsertAndExtractPortMembers) {
  {
    MachPort sendReceive(MachPort::Type::SendReceive);
    ASSERT_TRUE(sendReceive.isValid());
    MachPort toInsert(MachPort::Type::SendReceive);
    ASSERT_TRUE(toInsert.isValid());
    ASSERT_FALSE(sendReceive.insertMember(toInsert));
  }

  {
    MachPort portSet(MachPort::Type::PortSet);
    ASSERT_TRUE(portSet.isValid());
    MachPort toInsert(MachPort::Type::SendReceive);
    ASSERT_TRUE(toInsert.isValid());

    // Insertion.
    ASSERT_TRUE(portSet.insertMember(toInsert));

    // Already in set.
    ASSERT_FALSE(portSet.insertMember(toInsert));

    // Extraction.
    ASSERT_TRUE(portSet.extractMember(toInsert));

    // Already extracted.
    ASSERT_TRUE(portSet.extractMember(toInsert));

    // Insert again.
    ASSERT_TRUE(portSet.insertMember(toInsert));

    // Already in set.
    ASSERT_FALSE(portSet.insertMember(toInsert));
  }
}

// TODO: A variant of this test that sends attachments to a child process is
// required.
TEST(MachPortTest, CanSendAndReceiveAttachments) {
  MachPort port(MachPort::Type::SendReceive);

  // The port needs to be alive till it is read on the other end.
  auto portToSend = std::make_shared<MachPort>(MachPort::Type::SendReceive);

  {
    Message message;
    char c = 'c';
    ASSERT_TRUE(message.encode(c));
    ASSERT_TRUE(message.encode(portToSend));

    Messages messages;
    messages.emplace_back(std::move(message));
    ASSERT_EQ(port.sendMessages(std::move(messages), ClockDurationMilli(10)),
              IOResult::Success);
  }

  {
    auto result = port.receiveMessage(ClockDurationMilli(10));
    ASSERT_EQ(result.first, IOResult::Success);

    // No messages were in the kernel buffer. So this call should timeout.
    auto no_more_messages = port.receiveMessage(ClockDurationMilli(5));
    ASSERT_EQ(no_more_messages.first, IOResult::Timeout);

    auto& message = result.second;
    char c = '\0';
    ASSERT_TRUE(message.decode(c));
    ASSERT_EQ(c, 'c');

    RawAttachment attachment;
    ASSERT_TRUE(message.decode(attachment));
    MachPort port(std::move(attachment));
    ASSERT_TRUE(port.isValid());
  }
}

#endif  // RL_CHANNELS == RL_CHANNELS_MACH

}  // namespace testing
}  // namespace core
}  // namespace rl
