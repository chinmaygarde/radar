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
  static const size_t MaxBufferSize;
  static const size_t MaxControlBufferItemCount;
  static const size_t ControlBufferItemSize;
  static const size_t MaxControlBufferSize;

  typedef int Handle;

  typedef enum {
    Success = 0,
    TemporaryFailure,
    PermanentFailure,
  } Status;

  typedef std::pair<Status, std::vector<std::unique_ptr<Message>>> ReadResult;
  typedef std::pair<std::unique_ptr<Socket>, std::unique_ptr<Socket>> Pair;

  /*
   *  Creating a socket
   */

  static std::unique_ptr<Socket> Create(Handle Handle = 0);
  static Pair CreatePair();

  Socket() = delete;
  explicit Socket(Handle handle = 0);

  virtual ~Socket();

  /*
   *  Connecting and disconnecting sockets
   */

  bool connect(std::string endpoint);

  bool close();

  /*
   *  Reading and writing messages on sockets
   */

  Status WriteMessage(Message& message);
  ReadResult ReadMessages();

  Handle handle() const { return _handle; }

 protected:
  Lock _lock;

  uint8_t* _buffer;
  uint8_t* _controlBuffer;

  Handle _handle;
};
}

#endif /* defined(__RADARLOVE__SOCKETACCESSOR__) */
