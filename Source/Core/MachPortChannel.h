// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef RADARLOVE_CORE_CHANNEL_MACHPORTCHANNEL_
#define RADARLOVE_CORE_CHANNEL_MACHPORTCHANNEL_

#include <Core/Config.h>

#if RL_CHANNELS == RL_CHANNELS_MACH

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
  IOResult writeMessages(Messages&& message,
                         ClockDurationNano timeout) override;
  IOReadResult readMessage(ClockDurationNano timeout) override;
  Message::Attachment::Handle handle() override;
  bool doTerminate() override;

 private:
  Channel& _channel;
  MachPort _port;

  RL_DISALLOW_COPY_AND_ASSIGN(MachPortChannel);
};

}  // namespace core
}  // namespace rl

#endif  // RL_CHANNELS == RL_CHANNELS_MACH

#endif  // RADARLOVE_CORE_CHANNEL_MACHPORTCHANNEL_
