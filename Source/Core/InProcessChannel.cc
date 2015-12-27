// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <Core/Config.h>

#if RL_CHANNELS == RL_CHANNELS_INPROCESS

#include "InProcessChannel.h"

namespace rl {
namespace core {

InProcessChannel::InProcessChannel(Channel& owner) : _channel(owner) {}

InProcessChannel::InProcessChannel(Channel& owner,
                                   const Message::Attachment& attachment)
    : _channel(owner) {
  RL_ASSERT_MSG(false, "WIP");
}

InProcessChannel::~InProcessChannel() {
  RL_ASSERT(_activeWaitSets.size() == 0);
}

std::shared_ptr<EventLoopSource> InProcessChannel::createSource() const {
  using ELS = EventLoopSource;

  auto handle = reinterpret_cast<ELS::Handle>(this);
  auto allocator = [handle]() {
    /*
     *  Since this channel is going to write to this source as well are read
     *  from it, we assign ourselves as the read and write handles.
     */
    return ELS::Handles(handle, handle);
  };

  auto readHandler = [&](ELS::Handle handle) {
    return _channel.readPendingMessageNow();
  };

  auto updateHandler = [&](EventLoopSource& source, WaitSet& waitset,
                           ELS::Handle ident, bool adding) {
    if (adding) {
      _activeWaitSets.insert(&waitset);
    } else {
      _activeWaitSets.erase(&waitset);
    }
  };

  return std::make_shared<ELS>(allocator, nullptr, readHandler, nullptr,
                               updateHandler);
}

IOResult InProcessChannel::writeMessages(Messages&& messages,
                                         ClockDurationNano timeout) {
  if (messages.size() == 0) {
    return IOResult::Success;
  }

  std::lock_guard<std::mutex> lock(_lock);

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

IOReadResult InProcessChannel::readMessage(ClockDurationNano timeout) {
  std::lock_guard<std::mutex> lock(_lock);

  if (_messageBuffer.size() == 0) {
    return IOReadResult(IOResult::Timeout, Message{});
  }

  Message readMessage(std::move(_messageBuffer.front()));
  _messageBuffer.pop_front();

  return IOReadResult(IOResult::Success, std::move(readMessage));
}

Message::Attachment::Handle InProcessChannel::handle() {
  return reinterpret_cast<Message::Attachment::Handle>(this);
}

bool InProcessChannel::doTerminate() {
  _messageBuffer.clear();
  return true;
}

}  // namespace core
}  // namespace rl

#endif  // RL_CHANNELS == RL_CHANNELS_INPROCESS
