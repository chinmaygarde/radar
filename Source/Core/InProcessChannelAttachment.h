// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef RADARLOVE_CORE_INPROCESSCHANNELATTACHMENT_H_
#define RADARLOVE_CORE_INPROCESSCHANNELATTACHMENT_H_

#include <Core/Config.h>

#if RL_CHANNELS == RL_CHANNELS_INPROCESS

#include <Core/Macros.h>
#include <Core/InProcessAttachment.h>
#include <Core/Message.h>
#include <Core/WaitSet.h>

#include <mutex>
#include <vector>
#include <list>
#include <unordered_set>

namespace rl {
namespace core {

class Channel;

class InProcessChannelAttachment : public InProcessAttachment {
 public:
  InProcessChannelAttachment();

  IOResult writeMessages(Messages&& message, ClockDurationNano timeout);

  IOReadResult readMessage(ClockDurationNano timeout);

  void addSubscriberWaitset(WaitSet& waitset);

  void removeSubscriberWaitset(WaitSet& waitset);

  ~InProcessChannelAttachment();

 private:
  std::mutex _messageBufferMutex;
  std::list<Message> _messageBuffer;
  std::mutex _subscriberWaitsetsMutex;
  std::unordered_set<WaitSet*> _subscriberWaitSets;

  RL_DISALLOW_COPY_AND_ASSIGN(InProcessChannelAttachment);
};

}  // namespace core
}  // namespace rl

#endif  // RL_CHANNELS == RL_CHANNELS_INPROCESS

#endif  // RADARLOVE_CORE_INPROCESSCHANNELATTACHMENT_H_
