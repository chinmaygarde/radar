// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <gtest/gtest.h>
#include "Channel.h"
#include "Server.h"
#include "Looper.h"
#include "Message.h"

#include <thread>

#if !__APPLE__

TEST(ChannelTest, SimpleInitialization) {
  auto endpoint = "/tmp/radarlove_simple_client_server";

  rl::Channel channel(endpoint);
  ASSERT_TRUE(channel.isReady());
}

TEST(ChannelTest, SimpleConnection) {
  auto endpoint = "/tmp/radarlove_simple_client_server2";

  rl::Server server(endpoint);

  ASSERT_TRUE(server.isListening());

  std::thread serverThread([&] {
    auto looper = rl::Looper::Current();

    /*
     *  Step 1: Add the source listening for new connections
     *  on the server side
     */
    looper->addSource(server.clientConnectionsSource());

    /*
     *  Step 2: Setup the client that will terminate the server
     *  loop when it connects with the same.
     */
    rl::Channel channel(endpoint);
    ASSERT_TRUE(channel.isReady());

    std::thread clientThread([&] {
      for (int i = 0; i < 5; i++) {
        if (channel.tryConnect()) {
          break;
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(10));
      }

      ASSERT_TRUE(channel.isConnected());

      looper->terminate();
    });

    /*
     *  Step 3: Start looping the server
     */
    looper->loop();

    clientThread.join();
  });

  serverThread.join();
}

TEST(ChannelTest, SimpleConnectionSend) {
  auto endpoint = "/tmp/radarlove_simple_client_server3";

  rl::Server server(endpoint);

  std::shared_ptr<rl::Channel> serverChannelToClient;

  /*
   *  We need to keep the channel to the client open long enough for
   *  it to send us a message. If we depend on the implicit behavior
   *  of closing the connection, we may get into a racy condition where
   *  the ack has happened but the accept has not.
   */
  server.channelAvailabilityCallback(
      [&serverChannelToClient](std::shared_ptr<rl::Channel> channel) {
        serverChannelToClient.swap(channel);

        /*
         *  If the server only wanted one connected, it would terminate
         * these as it came along.
         */
      });

  ASSERT_TRUE(server.isListening());

  std::thread serverThread([&] {
    auto looper = rl::Looper::Current();

    /*
     *  Step 1: Add the source listening for new connections
     *  on the server side
     */
    looper->addSource(server.clientConnectionsSource());

    /*
     *  Step 2: Setup the client that will terminate the server
     *  loop when it connects with the same.
     */
    rl::Channel channel(endpoint);

    ASSERT_TRUE(channel.isReady());

    std::thread clientThread([&] {
      for (int i = 0; i < 5; i++) {
        if (channel.tryConnect()) {
          break;
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(10));
      }

      ASSERT_TRUE(channel.isConnected());

      rl::Message message;
      message.encode(10.0);
      message.encode(false);
      message.encode('c');

      ASSERT_TRUE(channel.sendMessage(message));

      looper->terminate();
    });

    /*
     *  Step 3: Start looping the server
     */
    looper->loop();

    clientThread.join();
  });

  serverThread.join();

  ASSERT_TRUE(serverChannelToClient.get() != nullptr);

  serverChannelToClient->terminate();
}

TEST(ChannelTest, TwoWayCommunicationTest) {
  auto endpoint = "/tmp/radarlove_simple_client_server4";

  int messagesReceived = 0;

  const int MessagesCount = 25;

  std::thread serverThread([endpoint, &messagesReceived]() {
    rl::Server server(endpoint);
    ASSERT_TRUE(server.isListening());

    std::shared_ptr<rl::Channel> clientToServerChannel;

    rl::Channel::MessageReceivedCallback messageCallback =
        [&clientToServerChannel, &messagesReceived](rl::Message& message) {
          bool a = true;
          ASSERT_TRUE(message.decode(a));
          ASSERT_TRUE(a == false);

          int i = 0;
          ASSERT_TRUE(message.decode(i));
          ASSERT_TRUE(i == messagesReceived);
          messagesReceived++;

          char c = 'a';
          ASSERT_TRUE(message.decode(c));
          ASSERT_TRUE(c == 'c');

          int d = 0;
          ASSERT_FALSE(message.decode(d));

          if (messagesReceived == MessagesCount) {
            clientToServerChannel->unscheduleFromLooper(rl::Looper::Current());
            rl::Looper::Current()->terminate();
          }
        };

    server.channelAvailabilityCallback(
        [&clientToServerChannel, &messageCallback](
            std::shared_ptr<rl::Channel> channel) {

          if (clientToServerChannel.get() != nullptr) {
            /*
             *  We only need one connection
             */
            channel->terminate();
            return;
          }

          clientToServerChannel.swap(channel);
          clientToServerChannel->messageReceivedCallback(messageCallback);
          clientToServerChannel->scheduleInLooper(rl::Looper::Current());
        });

    auto looper = rl::Looper::Current();
    looper->addSource(server.clientConnectionsSource());
    looper->loop();
  });

  std::thread clientThread([endpoint]() {
    rl::Channel channel(endpoint);
    ASSERT_TRUE(channel.isReady());

    for (int i = 0; i < 5; i++) {
      if (channel.tryConnect()) {
        break;
      }
      std::this_thread::sleep_for(std::chrono::milliseconds(5));
    }

    ASSERT_TRUE(channel.isConnected());

    for (int i = 0; i < MessagesCount; i++) {
      rl::Message message;

      message.encode(false);
      message.encode(i);
      message.encode('c');

      ASSERT_TRUE(channel.sendMessage(message));
    }
  });

  clientThread.join();
  serverThread.join();

  ASSERT_TRUE(messagesReceived == MessagesCount);
}

TEST(ChannelTest, CreateConnectedChannels) {
  auto connectedChannels = rl::Channel::CreateConnectedChannels();
  ASSERT_TRUE(connectedChannels.first.get() != nullptr);
  ASSERT_TRUE(connectedChannels.second.get() != nullptr);
}

#endif  // __APPLE__
