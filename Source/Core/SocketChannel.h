// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef __RADARLOVE_CORE_CHANNEL_SOCKETCHANNEL__
#define __RADARLOVE_CORE_CHANNEL_SOCKETCHANNEL__

#include <Core/Channel.h>
#include <Core/ChannelProvider.h>

#include <mutex>

namespace rl {
class SocketChannel : public ChannelProvider {
 public:
  using Handle = int;

  explicit SocketChannel(Channel& owner);

  ~SocketChannel();
  std::shared_ptr<EventLoopSource> createSource() const override;
  Result WriteMessages(Messages&& message) override;
  ReadResult ReadMessages() override;
  bool doTerminate() override;

 private:
  std::mutex _lock;
  uint8_t* _buffer;
  uint8_t* _controlBuffer;
  std::pair<Handle, Handle> _handles;
  Channel& _channel;

  Handle readHandle() const;
  Handle writeHandle() const;
  Result writeMessageSingle(const Message& message);
  Result writeMessageInline(const Message& message);
  Result writeMessageOutOfLine(const Message& message);

  RL_DISALLOW_COPY_AND_ASSIGN(SocketChannel);
};
}  // namespace rl

#endif /* defined(__RADARLOVE_CORE_CHANNEL_SOCKETCHANNEL__) */
