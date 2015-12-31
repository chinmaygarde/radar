// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef __RADARLOVE_CORE_CHANNEL_SOCKETCHANNEL__
#define __RADARLOVE_CORE_CHANNEL_SOCKETCHANNEL__

#include <Core/Channel.h>
#include <Core/ChannelProvider.h>

#include <mutex>

namespace rl {
namespace core {

class SocketChannel : public ChannelProvider {
 public:
  using Handle = int;

  explicit SocketChannel(Channel& owner);

  explicit SocketChannel(Channel& owner, const Message::Attachment& attachment);

  ~SocketChannel();
  std::shared_ptr<EventLoopSource> createSource() const override;
  IOResult writeMessages(Messages&& message,
                         ClockDurationNano timeout) override;
  IOReadResult readMessage(ClockDurationNano timeout) override;
  Message::Attachment::Handle handle() override;
  bool doTerminate() override;

 private:
  std::mutex _readBufferMutex;
  uint8_t* _buffer;
  uint8_t* _controlBuffer;
  std::pair<Handle, Handle> _handles;
  Channel& _channel;

  void setupWithHandles(Handle readHandle, Handle writeHandle);

  Handle readHandle() const;
  Handle writeHandle() const;

  IOResult writeMessageSingle(const Message& message,
                              ClockDurationNano timeout);

  RL_DISALLOW_COPY_AND_ASSIGN(SocketChannel);
};

}  // namespace core
}  // namespace rl

#endif /* defined(__RADARLOVE_CORE_CHANNEL_SOCKETCHANNEL__) */
