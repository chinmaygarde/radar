// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef __RADARLOVE_CORE_CHANNEL_SOCKETCHANNEL__
#define __RADARLOVE_CORE_CHANNEL_SOCKETCHANNEL__

#include "Core/Channel.h"
#include "Core/ChannelProvider.h"

#include <mutex>

namespace rl {
class SocketChannel : public ChannelProvider {
 public:
  using Handle = int;

  SocketChannel(Channel& owner);

  ~SocketChannel();
  virtual std::shared_ptr<LooperSource> createSource() const override;
  virtual Result WriteMessage(Message& message) override;
  virtual ReadResult ReadMessages() override;
  virtual bool doTerminate() override;

 private:
  std::mutex _lock;
  uint8_t* _buffer;
  uint8_t* _controlBuffer;
  std::pair<Handle, Handle> _handles;
  Channel& _channel;

  Handle readHandle() const;
  Handle writeHandle() const;

  DISALLOW_COPY_AND_ASSIGN(SocketChannel);
};
}

#endif /* defined(__RADARLOVE_CORE_CHANNEL_SOCKETCHANNEL__) */
