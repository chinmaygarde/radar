// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "InProcessChannel.h"

namespace rl {

InProcessChannel::InProcessChannel(Channel& owner) : _channel(owner) {
}

InProcessChannel::~InProcessChannel() {
}

std::shared_ptr<EventLoopSource> InProcessChannel::createSource() const {
  using ELS = EventLoopSource;

  auto handle = reinterpret_cast<ELS::Handle>(this);
  auto allocator = [&]() { return ELS::Handles(handle, handle); };
  auto readHandler =
      [&](ELS::Handle handle) { _channel.readPendingMessageNow(); };

  return std::make_shared<ELS>(allocator, nullptr, readHandler, nullptr,
                               nullptr);
}

ChannelProvider::Result InProcessChannel::WriteMessages(Messages&& messages) {
  if (messages.size() == 0) {
    return ChannelProvider::Result::Success;
  }

  std::lock_guard<std::mutex> lock(_lock);

  for (auto& message : messages) {
    message.rewindRead();
    _messageBuffer.push_back(std::move(message));
  }

  _conditionVariable.notify_all();

  return ChannelProvider::Result::Success;
}

ChannelProvider::ReadResult InProcessChannel::ReadMessages() {
  /*
   *  Hold a unique lock on the mutex. This constructor calls lock() on _lock
   */
  std::unique_lock<std::mutex> lock(_lock);

  _conditionVariable.wait(lock, [&] { return _messageBuffer.size() == 0; });

  Messages readMessages;

  while (_messageBuffer.size() > 0) {
    readMessages.push_back(std::move(_messageBuffer.front()));
    _messageBuffer.pop_front();
  }

  _lock.unlock();

  return ChannelProvider::ReadResult(
      readMessages.size() == 0 ? Result::TemporaryFailure : Result::Success,
      std::move(readMessages));
}

bool InProcessChannel::doTerminate() {
  _messageBuffer.clear();
  return true;
}

}  // namespace rl
