// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <Core/Channel.h>
#include <Core/ChannelProvider.h>
#include <Core/Config.h>
#include <Core/Message.h>
#include <Core/Utilities.h>

#include <Core/InProcessChannel.h>
#include <Core/MachPortChannel.h>
#include <Core/SocketChannel.h>

namespace rl {
namespace core {

template <typename... Args>
static std::unique_ptr<ChannelProvider> ChannelSelectProvider(Args&&... args) {
#if RL_CHANNELS == RL_CHANNELS_MACH
  return make_unique<MachPortChannel>(std::forward<Args>(args)...);
#elif RL_CHANNELS == RL_CHANNELS_SOCKET
  return make_unique<SocketChannel>(std::forward<Args>(args)...);
#elif RL_CHANNELS == RL_CHANNELS_INPROCESS
  return make_unique<InProcessChannel>(std::forward<Args>(args)...);
#else
#error Unknown Channels Implementation
  return nullptr;
#endif
}

Channel::Channel(const Message::Attachment& attachment)
    : _terminated(false), _provider(ChannelSelectProvider(*this, attachment)) {}

Channel::Channel()
    : _terminated(false), _provider(ChannelSelectProvider(*this)) {}

Channel::~Channel() {
  terminate();
}

void Channel::terminate() {
  if (_terminated) {
    return;
  }

  bool closed = _provider->doTerminate();
  RL_ASSERT(closed);

  _terminated = true;

  if (_terminationCallback) {
    _terminationCallback();
  }
}

IOResult Channel::sendMessages(Messages messages) {
  if (messages.size() == 0) {
    return IOResult::Success;
  }

  auto result =
      _provider->writeMessages(std::move(messages), ClockDurationNano::max());

  switch (result) {
    case IOResult::Failure:
      terminate();
      return IOResult::Failure;
    case IOResult::Success:
    case IOResult::Timeout:
      return result;
  }

  return IOResult::Failure;
}

const Channel::MessageCallback& Channel::messageCallback() const {
  return _messageCallback;
}

void Channel::setMessageCallback(MessageCallback callback) {
  _messageCallback = callback;
}

IOResult Channel::readPendingMessageNow() {
  auto result = _provider->readMessage(ClockDurationNano::max());

  switch (result.first) {
    case IOResult::Success:
      if (_messageCallback) {
        _messageCallback(std::move(result.second));
      }
      return IOResult::Success;
    case IOResult::Failure:
      terminate();
    case IOResult::Timeout:
      return result.first;
  }

  return IOResult::Failure;
}

Messages Channel::drainPendingMessages() {
  Messages messages;

  while (true) {
    auto result = _provider->readMessage(ClockDurationNano(0));

    switch (result.first) {
      case IOResult::Success:
        messages.emplace_back(std::move(result.second));
        continue;
      case IOResult::Timeout:
        goto Done;
      case IOResult::Failure:
        terminate();
        goto Done;
    }
  }

Done:
  return std::move(messages);
}

Message::Attachment Channel::asMessageAttachment() const {
  return _provider->handle();
}

Channel::TerminationCallback Channel::terminationCallback() const {
  return _terminationCallback;
}

void Channel::setTerminationCallback(Channel::TerminationCallback callback) {
  _terminationCallback = callback;
}

std::shared_ptr<EventLoopSource> Channel::source() {
  if (_source) {
    return _source;
  }

  _source = _provider->createSource();

  return _source;
}

}  // namespace core
}  // namespace rl
