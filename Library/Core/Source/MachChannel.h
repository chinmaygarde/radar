/*
 *  This source file is part of the Radar project.
 *  Licensed under the MIT License. See LICENSE file for details.
 */

#pragma once

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
  std::unique_ptr<MachPort> _set;
  std::shared_ptr<MachPort> _port;

  bool setupPortSetMemberships();
  bool teardownPortSetMemberships();

  RL_DISALLOW_COPY_AND_ASSIGN(MachChannel);
};

}  // namespace core
}  // namespace rl

#endif  // RL_CHANNELS == RL_CHANNELS_MACH
