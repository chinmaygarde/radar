// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef __RADARLOVE__CHANNEL__
#define __RADARLOVE__CHANNEL__

#include "Base.h"
#include "Looper.h"
#include "Socket.h"

#include <string>
#include <memory>

namespace rl {

class Message;
class Channel {
 public:
  typedef int Handle;

  typedef std::function<void(Message&)> MessageReceivedCallback;
  typedef std::function<void(void)> TerminationCallback;
  typedef std::pair<std::shared_ptr<Channel>, std::shared_ptr<Channel>>
      ConnectedChannels;

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

  /*
   *  Sending and receiving messages
   */

  bool sendMessage(Message& message);

  void messageReceivedCallback(MessageReceivedCallback callback) {
    _messageReceivedCallback = callback;
  }

  MessageReceivedCallback messageReceivedCallback() const {
    return _messageReceivedCallback;
  }

  /*
   *  Introspecting connection status
   */

  bool tryConnect();

  bool isConnected() const { return _connected; }

  bool isReady() const { return _ready; }

  void terminate();

  void terminationCallback(TerminationCallback callback) {
    _terminationCallback = callback;
  }

  TerminationCallback terminationCallback() const {
    return _terminationCallback;
  }

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
