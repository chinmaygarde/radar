// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef RADARLOVE_CORE_CHANNEL_SOCKETCHANNEL_
#define RADARLOVE_CORE_CHANNEL_SOCKETCHANNEL_

#include <Core/Config.h>

#if RL_CHANNELS == RL_CHANNELS_SOCKET

#include <Core/Allocation.h>
#include <Core/Channel.h>
#include <Core/ChannelProvider.h>
#include <Core/RawAttachment.h>

#include "SocketPair.h"

#include <mutex>

namespace rl {
namespace core {

class SocketChannel : public ChannelProvider {
 public:
  explicit SocketChannel(Channel& owner);

  explicit SocketChannel(Channel& owner, RawAttachment attachment);

  ~SocketChannel();

  std::shared_ptr<EventLoopSource> createSource() const override;

  IOResult writeMessages(Messages&& message,
                         ClockDurationNano timeout) override;

  IOReadResult readMessage(ClockDurationNano timeout) override;

  AttachmentRef attachment() override;

  bool doTerminate() override;

 private:
  Channel& _channel;
  SocketPair _pair;

  std::mutex _readBufferMutex;
  Allocation _inlineMessageBuffer;
  Allocation _controlBuffer;

  void setup();

  IOResult writeMessageSingle(const Message& message,
                              ClockDurationNano timeout);

  RL_DISALLOW_COPY_AND_ASSIGN(SocketChannel);
};

}  // namespace core
}  // namespace rl

#endif  // RL_CHANNELS == RL_CHANNELS_SOCKET

#endif  // RADARLOVE_CORE_CHANNEL_SOCKETCHANNEL_
