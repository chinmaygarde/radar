// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef __RADARLOVE_CORE_CHANNEL__
#define __RADARLOVE_CORE_CHANNEL__

#include <Core/Macros.h>
#include <Core/EventLoop.h>
#include <Core/Message.h>

#include <string>
#include <memory>
#include <vector>

namespace rl {

class ChannelProvider;

class Channel {
 public:
  using MessagesReceivedCallback = std::function<void(Messages)>;
  using TerminationCallback = std::function<void(void)>;

#pragma mark - Creating Channels
  /**
   *  Create a new channel
   */
  explicit Channel();

  virtual ~Channel();

#pragma mark - Sending and receiving messages on channels

  /**
   *  Sends the specified messages down the channel
   *
   *  @param messages the messages to send
   *
   *  @return if the messages were successfully sent
   */
  bool sendMessages(Messages message);

  /**
   *  When messages arrive on this channel, a callback may be invoked on the
   *  loop where this channel is scheduled. Get this callback
   *
   *  @return the message received callback
   */
  const MessagesReceivedCallback& messagesReceivedCallback() const;

  /**
   *  Update the callback that will be invoked when messages arrive on this
   *  channel. The channel must be scheduled in a loop
   *
   *  @param callback the new messages received callback
   */
  void setMessagesReceivedCallback(MessagesReceivedCallback callback);

#pragma mark - Manual channel connection status management

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

#pragma mark - Event Loopinteraction

  /**
   *  Get a loop source for this channel. Channels need to be scheduled on
   *  loops to get message and lifecycle callbacks
   *
   *  @return the loop source for this channel
   */
  std::shared_ptr<EventLoopSource> source();

#pragma mark - Manually reads

  /**
   *  Read all pending messages on the channel immediately. The user rarely (if
   *  ever) needs to call this. Instead, it is usually called by the channel
   *  provider when it detects an available read.
   */
  void readPendingMessageNow();

 private:
  MessagesReceivedCallback _messagesReceivedCallback;
  TerminationCallback _terminationCallback;
  bool _terminated;
  std::unique_ptr<ChannelProvider> _provider;
  std::shared_ptr<EventLoopSource> _source;

  RL_DISALLOW_COPY_AND_ASSIGN(Channel);
};
}

#endif  // __RADARLOVE_CORE_CHANNEL__
