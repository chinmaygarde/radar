// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <Core/Config.h>

#if RL_CHANNELS == RL_CHANNELS_INPROCESS

#include <Core/InProcessChannelAttachment.h>
#include <Core/InProcessWaitSet.h>
#include <Core/Channel.h>

#include <algorithm>

namespace rl {
namespace core {

InProcessChannelAttachment::InProcessChannelAttachment() {}

InProcessChannelAttachment::~InProcessChannelAttachment() {}

IOResult InProcessChannelAttachment::writeMessages(Messages&& messages,
                                                   ClockDurationNano) {
  /*
   *  There is no limit on the size of the message buffer. So the timeout is
   *  ignored. It may be that contention on lock for the message buffer itself
   *  causes an overflow of the allotted time. That case is not handled here.
   */
  RL_ASSERT(messages.size() != 0);

  bufferMessages(std::move(messages));

  signalReadReadinessOnUserspaceChannels();

  return IOResult::Success;
}

IOReadResult InProcessChannelAttachment::readMessage(ClockDurationNano) {
  /*
   *  WIP: Timeout must be respected
   */
  std::lock_guard<std::mutex> lock(_messageBufferMutex);

  if (_messageBuffer.size() == 0) {
    return IOReadResult(IOResult::Timeout, Message{});
  }

  Message readMessage(std::move(_messageBuffer.front()));
  _messageBuffer.pop_front();

  return IOReadResult(IOResult::Success, std::move(readMessage));
}

void InProcessChannelAttachment::addSubscriberWaitset(
    InProcessWaitSet& waitset) {
  std::lock_guard<std::mutex> lock(_subscriberWaitsetsMutex);
  _subscriberWaitSets.insert(&waitset);
}

void InProcessChannelAttachment::removeSubscriberWaitset(
    InProcessWaitSet& waitset) {
  std::lock_guard<std::mutex> lock(_subscriberWaitsetsMutex);
  _subscriberWaitSets.erase(&waitset);
}

void InProcessChannelAttachment::registerUserspaceChannel(Channel& channel) {
  std::lock_guard<std::mutex> lock(_userspaceChannelsMutex);
  _userspaceChannels.insert(&channel);
}

void InProcessChannelAttachment::unregisterUserspaceChannel(Channel& channel) {
  std::lock_guard<std::mutex> lock(_userspaceChannelsMutex);
  _userspaceChannels.erase(&channel);
}

IOResult InProcessChannelAttachment::bufferMessages(Messages&& messages) {
  std::lock_guard<std::mutex> lock(_messageBufferMutex);

  for (auto& message : messages) {
    message.rewindRead();
    _messageBuffer.push_back(std::move(message));
  }

  return IOResult::Success;
}

void InProcessChannelAttachment::signalReadReadinessOnUserspaceChannels() {
  std::vector<EventLoopSource::Handle> readyHandles;

  {
    /*
     *  Prepare a collection of all "userspace" handles that are ready.
     */
    std::lock_guard<std::mutex> channelLock(_userspaceChannelsMutex);
    for (Channel* channel : _userspaceChannels) {
      readyHandles.emplace_back(channel->source()->readHandle());
    }
  }

  {
    /*
     *  Signal to all the waitsets that are subscribed to these channels that
     *  they have channels ready to have data read from.
     */
    std::lock_guard<std::mutex> waitsetLock(_subscriberWaitsetsMutex);
    for (InProcessWaitSet* waitset : _subscriberWaitSets) {
      waitset->signalReadReadinessFromUserspace(readyHandles);
    }
  }
}

}  // namespace core
}  // namespace rl

#endif  // RL_CHANNELS == RL_CHANNELS_INPROCESS
