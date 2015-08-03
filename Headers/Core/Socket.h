// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef __RADARLOVE__SOCKETACCESSOR__
#define __RADARLOVE__SOCKETACCESSOR__

#include "Core/Base.h"
#include "Core/AutoLock.h"

#include <utility>
#include <string>
#include <vector>
#include <sys/socket.h>
#include <stdint.h>

namespace rl {

class Message;

class Socket {
 public:
  enum Result {
    Success = 0,
    TemporaryFailure,
    PermanentFailure,
  };

  using Handle = int;
  using ReadResult = std::pair<Result, std::vector<std::unique_ptr<Message>>>;
  using Pair = std::pair<std::unique_ptr<Socket>, std::unique_ptr<Socket>>;

#pragma mark - Creating a Socket

  /**
   *  Create a socket from a raw socket handle if necessary. Connecting two
   *  sockets manually is usually a pain in the ass. So if you just need a
   *  pair of connected sockets, use the `CreatePair` method instead.
   *
   *  @param handle the raw socket handle if available. Otherwise, a handle is
   *                created for you.
   */
  explicit Socket(Handle handle = 0);

  /**
   *  Create a connected pair of sockets
   *
   *  @return pair the pair of connected sockets
   */
  static Pair CreatePair();

  virtual ~Socket();

#pragma mark - Manually connecting sockets

  /**
   *  Connect the socket to a named endpoint
   *
   *  @param endpoint the name of the endpoint to connect to
   *
   *  @return if the socket connection was successful
   */
  bool connect(std::string endpoint);

  /**
   *  Close the socket
   *
   *  @return if the socket was successfully closed
   */
  bool close();

#pragma mark - Reading and writing messages on a socket

  /**
   *  Write a message on the socket
   *
   *  @param message the message to write
   *
   *  @return the write result
   */
  Result WriteMessage(Message& message);

  /**
   *  Read a message on the socket
   *
   *  @return the read result
   */
  ReadResult ReadMessages();

#pragma mark - Misc.

  Handle handle() const;

 protected:
  Lock _lock;
  uint8_t* _buffer;
  uint8_t* _controlBuffer;
  Handle _handle;

  DISALLOW_COPY_AND_ASSIGN(Socket);
};
}

#endif /* defined(__RADARLOVE__SOCKETACCESSOR__) */
