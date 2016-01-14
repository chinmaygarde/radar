// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef __RADARLOVE_CORE_INPROCESSCHANNEL_H__
#define __RADARLOVE_CORE_INPROCESSCHANNEL_H__

#include <Core/Config.h>

#if RL_CHANNELS == RL_CHANNELS_INPROCESS

#include <Core/Channel.h>

#include "ChannelProvider.h"

#include <condition_variable>
#include <list>
#include <unordered_set>

namespace rl {
namespace core {

class InProcessChannel : public ChannelProvider {
 public:
  explicit InProcessChannel(Channel& owner);
  explicit InProcessChannel(Channel& owner,
                            const Message::Attachment& attachment);

  ~InProcessChannel();

  std::shared_ptr<EventLoopSource> createSource() const override;
  IOResult writeMessages(Messages&& messages,
                         ClockDurationNano timeout) override;
  IOReadResult readMessage(ClockDurationNano timeout) override;
  Message::Attachment::Handle handle() override;
  bool doTerminate() override;

 private:
  std::mutex _lock;
  Channel& _channel;
  std::list<Message> _messageBuffer;
  mutable std::unordered_set<WaitSet*> _activeWaitSets;

  RL_DISALLOW_COPY_AND_ASSIGN(InProcessChannel);
};

}  // namespace core
}  // namespace rl

#endif  // RL_CHANNELS == RL_CHANNELS_INPROCESS

#endif  // __RADARLOVE_CORE_INPROCESSCHANNEL_H__
