// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef __RADARLOVE__CHANNEL__
#define __RADARLOVE__CHANNEL__

#include "Core/Base.h"
#include "Core/Looper.h"

#include <string>
#include <memory>
#include <vector>

namespace rl {

class Message;
class Channel {
 public:
  using MessageReceivedCallback = std::function<void(Message&)>;
  using TerminationCallback = std::function<void(void)>;
  using ConnectedPair =
      std::pair<std::shared_ptr<Channel>, std::shared_ptr<Channel>>;

#pragma mark - Creating Channels
  /**
   *  Create a channel to a named endpoint. Connection
   *  must be explicitly setup by the caller
   *
   *  @param name the name of the endopoint
   *
   *  @return the channel
   */
  explicit Channel(std::string name);

  /**
   *  Create a pair of connected channels
   *
   *  @return the connected channel pair
   */
  static ConnectedPair CreateConnectedPair();

  virtual ~Channel();

#pragma mark - Sending and receiving messages on channels

  /**
   *  Sends the specified message down the channel
   *
   *  @param message the message to send
   *
   *  @return if the message was successfully sent
   */
  bool sendMessage(Message& message);

  /**
   *  When messages arrive on this channel, a callback may be invoked on the
   *  looper where this channel is scheduled. Get this callback
   *
   *  @return the message received callback
   */
  const MessageReceivedCallback& messageReceivedCallback() const;

  /**
   *  Update the callback that will be invoked when messages arrive on this
   *  channel. The channel must be scheduled in a looper
   *
   *  @param callback the new messages received callback
   */
  void setMessageReceivedCallback(MessageReceivedCallback callback);

#pragma mark - Manual channel connection status management

  /**
   *  Try to establish the channel connection if not already setup
   *
   *  @return if the connection was made
   */
  bool connect();

  /**
   *  If the connection was already made
   *
   *  @return if the connection was made
   */
  bool isConnected() const;

  /**
   *  Terminate the channel connection and cleanup underlying resources
   */
  void terminate();

  /**
   *  Get the termination callback
   *
   *  @return the termination callback
   */
  TerminationCallback terminationCallback() const;

  /**
   *  Set the termination callback
   *
   *  @param callback the termination callback
   */
  void setTerminationCallback(TerminationCallback callback);

#pragma mark - Interacting with loopers

  /**
   *  Get a looper source for this channel. Channels need to be scheduled on
   *  loopers to get message and lifecycle callbacks
   *
   *  @return the looper source for this channel
   */
  virtual std::shared_ptr<LooperSource> source() = 0;

 protected:
  enum Result {
    Success = 0,
    TemporaryFailure,
    PermanentFailure,
  };

  using ReadResult = std::pair<Result, std::vector<std::unique_ptr<Message>>>;

  /**
   *  Write a message on the underlying channel implementation
   *
   *  @param message the message to write
   *
   *  @return the write result
   */
  virtual Result WriteMessage(Message& message) = 0;

  /**
   *  Read a message on the underlying channel implementation
   *
   *  @return the read result
   */
  virtual ReadResult ReadMessages() = 0;

  /**
   *  Perform actual channel connection
   *
   *  @return if the connection was successful
   */
  virtual bool doConnect(const std::string& endpoint) = 0;

  /**
   *  Perform the actual channel connection termination
   *
   *  @return if the termination was successful
   */
  virtual bool doTerminate() = 0;

  /**
   *  After the availability of messages on the underlying implementation is
   *  detected, this the base class calls this method so that the underlying
   *  implementation may perform the actual reads.
   */
  void readPendingMessageNow();

 private:
  MessageReceivedCallback _messageReceivedCallback;
  TerminationCallback _terminationCallback;
  bool _terminated;
  bool _connected;
  std::string _name;
};
}

#endif /* defined(__RADARLOVE__CHANNEL__) */
