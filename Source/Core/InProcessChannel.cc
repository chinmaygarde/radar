// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "InProcessChannel.h"

namespace rl {

InProcessChannel::InProcessChannel(Channel& owner) : _channel(owner) {
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
  auto readHandler =
      [&](ELS::Handle handle) { _channel.readPendingMessageNow(); };
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

ChannelProvider::Result InProcessChannel::WriteMessages(
    Messages&& messages,
    ClockDurationNano timeout) {
  if (messages.size() == 0) {
    return ChannelProvider::Result::Success;
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

  return ChannelProvider::Result::Success;
}

ChannelProvider::ReadResult InProcessChannel::ReadMessages(
    ClockDurationNano timeout) {
  std::lock_guard<std::mutex> lock(_lock);

  Messages readMessages;

  while (_messageBuffer.size() > 0) {
    readMessages.push_back(std::move(_messageBuffer.front()));
    _messageBuffer.pop_front();
  }

  return ChannelProvider::ReadResult(
      readMessages.size() == 0 ? Result::TemporaryFailure : Result::Success,
      std::move(readMessages));
}

bool InProcessChannel::doTerminate() {
  _messageBuffer.clear();
  return true;
}

}  // namespace rl
