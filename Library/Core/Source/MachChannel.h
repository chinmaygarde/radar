// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef RADAR_CORE_MACHCHANNEL_H_
#define RADAR_CORE_MACHCHANNEL_H_

#include "Config.h"

#if RL_CHANNELS == RL_CHANNELS_MACH

#include <Core/Channel.h>
#include <Core/ChannelProvider.h>

#include "MachPort.h"

namespace rl {
namespace core {

class MachChannel : public ChannelProvider {
 public:
  MachChannel(Channel& owner);

  MachChannel(Channel& owner, RawAttachment attachment);

  ~MachChannel();

  std::shared_ptr<EventLoopSource> createSource() const override;

  IOResult writeMessages(Messages&& message,
                         ClockDurationNano timeout) override;

  IOReadResult readMessage(ClockDurationNano timeout) override;

  AttachmentRef attachment() override;

  bool doTerminate() override;

 private:
  Channel& _channel;
  std::shared_ptr<MachPort> _port;
  std::unique_ptr<MachPort> _set;

  bool setupPortSetMemberships();
  bool teardownPortSetMemberships();

  RL_DISALLOW_COPY_AND_ASSIGN(MachChannel);
};

}  // namespace core
}  // namespace rl

#endif  // RL_CHANNELS == RL_CHANNELS_MACH

#endif  // RADAR_CORE_MACHCHANNEL_H_
