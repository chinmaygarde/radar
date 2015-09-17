// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef __RADARLOVE_CORE_CHANNEL_MACHPORTCHANNEL__
#define __RADARLOVE_CORE_CHANNEL_MACHPORTCHANNEL__

#include <Core/Channel.h>

namespace rl {
class MachPortChannel : public ChannelProvider {
 public:
  using Handle = uint32_t;

  MachPortChannel(Channel& owner);
  ~MachPortChannel();
  virtual std::shared_ptr<EventLoopSource> createSource() const override;
  virtual Result WriteMessages(Messages&& message) override;
  virtual ReadResult ReadMessages() override;
  virtual bool doTerminate() override;

 private:
  Handle _setHandle;
  Handle _handle;
  Channel& _channel;

  RL_DISALLOW_COPY_AND_ASSIGN(MachPortChannel);
};
}  // namespace rl

#endif /* defined(__RADARLOVE_CORE_CHANNEL_MACHPORTCHANNEL__) */
