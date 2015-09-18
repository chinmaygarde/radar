// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef __RADARLOVE_CORE_INPROCESSCHANNEL_H__
#define __RADARLOVE_CORE_INPROCESSCHANNEL_H__

#include <Core/Channel.h>

#include "ChannelProvider.h"

#include <condition_variable>
#include <list>
#include <unordered_set>

namespace rl {

class InProcessChannel : public ChannelProvider {
 public:
  InProcessChannel(Channel& owner);
  ~InProcessChannel();

  virtual std::shared_ptr<EventLoopSource> createSource() const override;
  virtual Result WriteMessages(Messages&& messages) override;
  virtual ReadResult ReadMessages() override;
  virtual bool doTerminate() override;

 private:
  std::mutex _lock;
  Channel& _channel;
  std::list<Message> _messageBuffer;
  mutable std::unordered_set<WaitSet*> _activeWaitSets;

  RL_DISALLOW_COPY_AND_ASSIGN(InProcessChannel);
};

}  // namespace rl

#endif  // __RADARLOVE_CORE_INPROCESSCHANNEL_H__
