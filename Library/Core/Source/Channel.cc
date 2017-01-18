// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <Core/Channel.h>
#include <Core/ChannelProvider.h>
#include <Core/Message.h>
#include <Core/Utilities.h>
#include "Config.h"
#include "InProcessChannel.h"
#include "MachChannel.h"
#include "SocketChannel.h"

namespace rl {
namespace core {

using PlatformChannelProvider =
#if RL_CHANNELS == RL_CHANNELS_MACH
    MachChannel
#elif RL_CHANNELS == RL_CHANNELS_SOCKET
    SocketChannel
#elif RL_CHANNELS == RL_CHANNELS_INPROCESS
    InProcessChannel
#else
#error Unknown Channels Implementation
#endif
    ;

Channel::Channel()
    : _localNS(nullptr),
      _terminated(false),
      _provider(core::make_unique<PlatformChannelProvider>(*this)) {}

Channel::Channel(RawAttachment attachment)
    : _localNS(nullptr),
      _terminated(false),
      _provider(
          core::make_unique<PlatformChannelProvider>(*this,
                                                     std::move(attachment))) {}

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
    _terminationCallback(*this);
  }
}

IOResult Channel::sendMessages(Messages messages, ClockDurationNano timeout) {
  if (messages.size() == 0) {
    return IOResult::Success;
  }

  auto result = _provider->writeMessages(std::move(messages), timeout);

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

IOResult Channel::readPendingMessageNow(ClockDurationNano timeout) {
  auto result = _provider->readMessage(timeout);

  switch (result.first) {
    case IOResult::Success:
      if (_messageCallback) {
        _messageCallback(std::move(result.second), _localNS);
      }
      return IOResult::Success;
    case IOResult::Failure:
      terminate();
    case IOResult::Timeout:
      return result.first;
  }

  return IOResult::Failure;
}

Messages Channel::drainPendingMessages(ClockDurationNano timeout,
                                       size_t limit) {
  Messages messages;

  while (true) {
    auto result = _provider->readMessage(timeout);

    switch (result.first) {
      case IOResult::Success:
        messages.emplace_back(std::move(result.second));
        if (messages.size() >= limit) {
          return messages;
        } else {
          continue;
        }
      case IOResult::Timeout:
        return messages;
      case IOResult::Failure:
        terminate();
        return messages;
    }
  }
}

AttachmentRef Channel::attachment() const {
  return _provider->attachment();
}

void Channel::setMessagesNamespace(Namespace* ns) {
  _localNS = ns;
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
