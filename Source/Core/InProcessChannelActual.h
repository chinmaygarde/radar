// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef RADARLOVE_CORE_INPROCESSCHANNELACTUAL_H_
#define RADARLOVE_CORE_INPROCESSCHANNELACTUAL_H_

#include <Core/Config.h>

#if RL_CHANNELS == RL_CHANNELS_INPROCESS

#include <Core/Macros.h>
#include <Core/ChannelProvider.h>

#include <mutex>
#include <vector>
#include <list>
#include <unordered_set>

namespace rl {
namespace core {

class Channel;

class InProcessChannelActual : public ChannelProvider {
 public:
  InProcessChannelActual();

  std::shared_ptr<EventLoopSource> createSource() const override;

  IOResult writeMessages(Messages&& message,
                         ClockDurationNano timeout) override;

  IOReadResult readMessage(ClockDurationNano timeout) override;

  bool doTerminate() override;

  Message::Attachment::Handle handle() override;

  void addUserspaceCounterpart(Channel& channel);

  void removeUserspaceCounterpart(Channel& channel);

  ~InProcessChannelActual() override;

 private:
  mutable std::mutex _userspaceChannelsMutex;
  std::vector<Channel*> _userspaceChannels;
  std::mutex _messageBufferMutex;
  std::list<Message> _messageBuffer;
  mutable std::unordered_set<WaitSet*> _activeWaitSets;

  Channel* randomChannel() const;

  RL_DISALLOW_COPY_AND_ASSIGN(InProcessChannelActual);
};

}  // namespace core
}  // namespace rl

#endif  // RL_CHANNELS == RL_CHANNELS_INPROCESS

#endif  // RADARLOVE_CORE_INPROCESSCHANNELACTUAL_H_
