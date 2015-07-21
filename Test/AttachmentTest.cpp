// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "Attachment.h"
#include "Channel.h"
#include "Message.h"
#include "SharedMemory.h"

#include <gtest/gtest.h>
#include <thread>

#if !__APPLE__

TEST(AttachmentTest, SingleAttachment) {
  auto channels = rl::Channel::CreateConnectedChannels();

  std::thread serverThread([&channels]() {

    /*
     *  Put the message in shared memory
     */
    const char hello[] = "hello";

    rl::SharedMemory memory(sizeof(hello));

    memcpy(memory.address(), hello, sizeof(hello));

    /*
     *  Create and send the shared memory handle as an attachment
     */
    rl::Message message;

    rl::Attachment attachment(memory.handle());
    message.addAttachment(attachment);

    ASSERT_TRUE(channels.first->sendMessage(message));
  });

  std::thread cliendThread([&channels]() {

    auto& channel = channels.second;

    channel->messageReceivedCallback([](rl::Message& message) {
      auto attachmentRange = message.attachmentRange();

      auto count = attachmentRange.second - attachmentRange.first;
      ASSERT_TRUE(count == 1);

      rl::Looper::Current()->terminate();
    });

    auto looper = rl::Looper::Current();

    channel->scheduleInLooper(looper);
    looper->loop();
  });

  serverThread.join();
  cliendThread.join();
}

#endif  // __APPLE__