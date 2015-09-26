// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef __RADARLOVE_CORE_CHANNEL_MACHPORTCHANNEL__
#define __RADARLOVE_CORE_CHANNEL_MACHPORTCHANNEL__

#include <Core/Channel.h>

#include "ChannelProvider.h"

namespace rl {
class MachPortChannel : public ChannelProvider {
 public:
  using Handle = uint32_t;

  explicit MachPortChannel(Channel& owner);
  ~MachPortChannel();
  std::shared_ptr<EventLoopSource> createSource() const override;
  Result WriteMessages(Messages&& message) override;
  ReadResult ReadMessages() override;
  bool doTerminate() override;

 private:
  Handle _setHandle;
  Handle _handle;
  Channel& _channel;

  RL_DISALLOW_COPY_AND_ASSIGN(MachPortChannel);
};
}  // namespace rl

#endif /* defined(__RADARLOVE_CORE_CHANNEL_MACHPORTCHANNEL__) */
