// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef __RADARLOVE__CHANNEL__
#define __RADARLOVE__CHANNEL__

#include "Core/Base.h"
#include "Core/Looper.h"
#include "Core/Socket.h"

#include <string>
#include <memory>

namespace rl {

class Message;
class Channel {
 public:
  using Handle = int;
  using MessageReceivedCallback = std::function<void(Message&)>;
  using TerminationCallback = std::function<void(void)>;
  using ConnectedChannels =
      std::pair<std::shared_ptr<Channel>, std::shared_ptr<Channel>>;

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
   *  Create a channel using a raw channel handle
   *  Ownership of the handle is assumed by the channel
   *  object
   *
   *  @param socketHandle the socket handle
   *
   *  @return the channel
   */
  explicit Channel(Handle socketHandle);

  /**
   *  Create a channel using a preallocated socket
   *  that is already connected
   *
   *  @param socket the socket object
   *
   *  @return the channel
   */
  explicit Channel(std::unique_ptr<Socket> socket);

  /**
   *  Create a pair of connected channels
   *
   *  @return the connected channel pair
   */
  static ConnectedChannels CreateConnectedChannels();

  ~Channel();

  /**
   *  Adds the channel to the specified looper.
   *  It is programmer error to collect the channel
   *  before it is unscheduled from all loopers via
   *  `unscheduleFromLooper`
   *
   *  @param looper the looper
   */
  void scheduleInLooper(Looper* looper);

  /**
   *  Unschedule the channel from the specified looper.
   *
   *  @param looper the looper
   */
  void unscheduleFromLooper(Looper* looper);

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

  /**
   *  Try to establish the channel connection if not already setup
   *
   *  @return if the connection was made
   */
  bool tryConnect();

  /**
   *  If the connection was already made
   *
   *  @return if the connection was made
   */
  bool isConnected() const;

  /**
   *  If the channel is ready for a connection
   *
   *  @return if the channel is ready for connection
   */
  bool isReady() const;

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

 private:
  std::shared_ptr<LooperSource> source();
  std::shared_ptr<LooperSource> _source;
  MessageReceivedCallback _messageReceivedCallback;
  TerminationCallback _terminationCallback;
  std::unique_ptr<Socket> _socket;
  bool _ready;
  bool _connected;
  std::string _name;

  void readMessageOnHandle(Handle handle);

  DISALLOW_COPY_AND_ASSIGN(Channel);
};
}

#endif /* defined(__RADARLOVE__CHANNEL__) */
