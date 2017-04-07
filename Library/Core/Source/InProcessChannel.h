// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#pragma once

#include "Config.h"

#if RL_CHANNELS == RL_CHANNELS_INPROCESS

#include <Core/ChannelProvider.h>

namespace rl {
namespace core {

class Channel;
class InProcessChannelAttachment;

class InProcessChannel : public ChannelProvider {
 public:
  InProcessChannel(Channel& owner);

  InProcessChannel(Channel& owner, RawAttachment attachment);

  ~InProcessChannel() override;

  std::shared_ptr<EventLoopSource> createSource() const override;

  IOResult writeMessages(Messages&& messages,
                         ClockDurationNano timeout) override;

  IOReadResult readMessage(ClockDurationNano timeout) override;

  AttachmentRef attachment() override;

  bool doTerminate() override;

 private:
  std::shared_ptr<InProcessChannelAttachment> _attachment;
  Channel& _owner;

  RL_DISALLOW_COPY_AND_ASSIGN(InProcessChannel);
};

}  // namespace core
}  // namespace rl

#endif  // RL_CHANNELS == RL_CHANNELS_INPROCESS
