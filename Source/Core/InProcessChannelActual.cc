// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <Core/Config.h>

#if RL_CHANNELS == RL_CHANNELS_INPROCESS

#include <Core/InProcessChannelActual.h>
#include <Core/Channel.h>

#include <algorithm>

namespace rl {
namespace core {

InProcessChannelActual::InProcessChannelActual() {}

InProcessChannelActual::~InProcessChannelActual() {
  RL_ASSERT(_userspaceChannels.size() == 0);
  RL_ASSERT(_activeWaitSets.size() == 0);
}

std::shared_ptr<EventLoopSource> InProcessChannelActual::createSource() const {
  using ELS = EventLoopSource;

  auto handle = reinterpret_cast<ELS::Handle>(this);
  auto allocator = [handle]() {
    /*
     *  Since this channel is going to write to this source as well are read
     *  from it, we assign ourselves as the read and write handles.
     */
    return ELS::Handles(handle, handle);
  };

  auto readHandler = [&](ELS::Handle) {
    auto channel = randomChannel();
    return channel != nullptr ? channel->readPendingMessageNow()
                              : IOResult::Failure;
  };

  auto updateHandler = [&](EventLoopSource&, WaitSet& waitset, ELS::Handle,
                           bool adding) {
    if (adding) {
      _activeWaitSets.insert(&waitset);
    } else {
      _activeWaitSets.erase(&waitset);
    }
  };

  return std::make_shared<ELS>(allocator, nullptr, readHandler, nullptr,
                               updateHandler);
}

IOResult InProcessChannelActual::writeMessages(Messages&& messages,
                                               ClockDurationNano) {
  /*
   *  There is no limit on the size of the message buffer. So the timeout is
   *  ignored. It may be that contention on lock for the message buffer itself
   *  causes an overflow of the allotted time. That case is not handled here.
   */
  if (messages.size() == 0) {
    return IOResult::Success;
  }

  std::lock_guard<std::mutex> lock(_messageBufferMutex);

  for (auto& message : messages) {
    message.rewindRead();
    _messageBuffer.push_back(std::move(message));
  }

  for (const auto& waitset : _activeWaitSets) {
    waitset->signalReadReadinessFromUserspace(
        reinterpret_cast<EventLoopSource::Handle>(this));
  }

  return IOResult::Success;
}

IOReadResult InProcessChannelActual::readMessage(ClockDurationNano) {
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

Message::Attachment::Handle InProcessChannelActual::handle() {
  RL_ASSERT_MSG(false, "Cannot access the handle of a non-userspace resource");
  return 0;
}

bool InProcessChannelActual::doTerminate() {
  _messageBuffer.clear();
  return true;
}

void InProcessChannelActual::addUserspaceCounterpart(Channel& channel) {
  std::lock_guard<std::mutex> lock(_userspaceChannelsMutex);

  auto found =
      std::find(_userspaceChannels.begin(), _userspaceChannels.end(), &channel);

  if (found == _userspaceChannels.end()) {
    _userspaceChannels.emplace_back(&channel);
  }
}

void InProcessChannelActual::removeUserspaceCounterpart(Channel& channel) {
  std::lock_guard<std::mutex> lock(_userspaceChannelsMutex);

  auto found =
      std::find(_userspaceChannels.begin(), _userspaceChannels.end(), &channel);

  if (found != _userspaceChannels.end()) {
    _userspaceChannels.erase(found);
  }
}

Channel* InProcessChannelActual::randomChannel() const {
  std::lock_guard<std::mutex> lock(_userspaceChannelsMutex);
  auto size = _userspaceChannels.size();
  if (size == 0) {
    return nullptr;
  }
  return _userspaceChannels[rand() % size];
}

}  // namespace core
}  // namespace rl

#endif  // RL_CHANNELS == RL_CHANNELS_INPROCESS
