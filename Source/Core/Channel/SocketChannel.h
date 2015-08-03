// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef __RADARLOVE_CORE_CHANNEL_SOCKETCHANNEL__
#define __RADARLOVE_CORE_CHANNEL_SOCKETCHANNEL__

#include "Core/Channel.h"

namespace rl {
class SocketChannel : public Channel {
 public:
  SocketChannel(const std::string& name);

  static ConnectedPair CreateConnectedPair();

  virtual ~SocketChannel() override;

  virtual std::shared_ptr<LooperSource> source() override;

 private:
  using Handle = int;

  std::mutex _lock;
  uint8_t* _buffer;
  uint8_t* _controlBuffer;
  Handle _handle;
  std::shared_ptr<LooperSource> _source;

  template <typename... T>
  static std::shared_ptr<SocketChannel> Shared(T&&... args);
  SocketChannel(const std::string& name, Handle handleOrZero);
  virtual Result WriteMessage(Message& message) override;
  virtual ReadResult ReadMessages() override;
  virtual bool doConnect(const std::string& endpoint) override;
  virtual bool doTerminate() override;

  DISALLOW_COPY_AND_ASSIGN(SocketChannel);
};
}

#endif /* defined(__RADARLOVE_CORE_CHANNEL_SOCKETCHANNEL__) */
