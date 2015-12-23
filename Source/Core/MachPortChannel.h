// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef __RADARLOVE_CORE_CHANNEL_MACHPORTCHANNEL__
#define __RADARLOVE_CORE_CHANNEL_MACHPORTCHANNEL__

#include <Core/Channel.h>

#include <Core/ChannelProvider.h>
#include <Core/MachPort.h>

namespace rl {
namespace core {

class MachPortChannel : public ChannelProvider {
 public:
  explicit MachPortChannel(Channel& owner);
  explicit MachPortChannel(Channel& owner,
                           const Message::Attachment& attachment);
  ~MachPortChannel();

  std::shared_ptr<EventLoopSource> createSource() const override;
  Result writeMessages(Messages&& message, ClockDurationNano timeout) override;
  ReadResult readMessage(ClockDurationNano timeout) override;
  Message::Attachment::Handle handle() override;
  bool doTerminate() override;

 private:
  Channel& _channel;
  MachPort _port;

  RL_DISALLOW_COPY_AND_ASSIGN(MachPortChannel);
};

}  // namespace core
}  // namespace rl

#endif /* defined(__RADARLOVE_CORE_CHANNEL_MACHPORTCHANNEL__) */
