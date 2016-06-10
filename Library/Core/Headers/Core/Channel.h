// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef RADARLOVE_CORE_CHANNEL_
#define RADARLOVE_CORE_CHANNEL_

#include <Core/EventLoop.h>
#include <Core/IOResult.h>
#include <Core/Macros.h>
#include <Core/Message.h>
#include <Core/RawAttachment.h>

#include <memory>
#include <string>
#include <vector>

namespace rl {
namespace core {

class ChannelProvider;

/**
 *  The core communication mechanism between the various subsystems in Radar.
 *  Channels facilitate the transactional transfer or data and attachments
 *  from one context to another. Attachments may be other channels or indeed
 *  any platform descriptor.
 */
class Channel {
 public:
  using MessageCallback = std::function<void(Message, Namespace*)>;
  using TerminationCallback = std::function<void(Channel&)>;

  /**
   *  Create a new channel
   */
  explicit Channel();

  /**
   *  Create a channel from a message attachment
   *
   *  @param attachment the attachment. Must be valid.
   */
  explicit Channel(RawAttachment attachment);

  /**
   *  The channel destructor
   */
  virtual ~Channel();

  /**
   *  Sends the specified messages down the channel
   *
   *  @param messages the messages to send
   *  @param timeout  the timeout on the message write
   *
   *  @return the result of attempting to send the message
   */
  IOResult sendMessages(Messages message,
                        ClockDurationNano timeout = ClockDurationNano::max());

  /**
   *  When a message arrive on this channel, a callback may be invoked on the
   *  loop where this channel is scheduled. Get this callback.
   *
   *  @return the message received callback
   */
  const MessageCallback& messageCallback() const;

  /**
   *  Update the callback that will be invoked when a message arrives on this
   *  channel. The channel must be scheduled in a loop.
   *
   *  @param callback the new message received callback.
   */
  void setMessageCallback(MessageCallback callback);

  /**
   *  Terminate the channel connection and cleanup underlying resources.
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

  /**
   *  Get a loop source for this channel. Channels need to be scheduled on
   *  loops to get message and lifecycle callbacks
   *
   *  @return the loop source for this channel
   */
  std::shared_ptr<EventLoopSource> source();

  /**
   *  Read all pending messages on the channel immediately. The user rarely (if
   *  ever) needs to call this. Instead, it is usually called by the channel
   *  provider when it detects an available read.
   *
   *  @return the result of attempting to read the message
   */
  IOResult readPendingMessageNow(
      ClockDurationNano timeout = ClockDurationNano::max());

  /**
   *  Directly drain all pending messages on this channel bypassing all the
   *  message callbacks
   *
   *  @return the messages on the channel that were drained
   */
  Messages drainPendingMessages();

  /**
   *  @return get the channel representation as a message attachment
   */
  AttachmentRef attachment() const;

  void setMessagesNamespace(Namespace* ns);

 private:
  Namespace* _localNS;
  MessageCallback _messageCallback;
  TerminationCallback _terminationCallback;
  bool _terminated;
  std::unique_ptr<ChannelProvider> _provider;
  std::shared_ptr<EventLoopSource> _source;

  RL_DISALLOW_COPY_AND_ASSIGN(Channel);
};

}  // namespace core
}  // namespace rl

#endif  // RADARLOVE_CORE_CHANNEL_
