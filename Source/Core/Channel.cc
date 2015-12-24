// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <Core/Channel.h>
#include <Core/Config.h>
#include <Core/Message.h>
#include <Core/Utilities.h>

#include "InProcessChannel.h"
#include "MachPortChannel.h"
#include "SocketChannel.h"

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

bool Channel::sendMessages(Messages messages) {
  if (messages.size() == 0) {
    return true;
  }

  /*
   *  All messages must be ready to sent down this channel. This is possibly
   *  a check too paranoid.
   */
  for (const auto& message : messages) {
    if (!message.isValid()) {
      return false;
    }
  }

  auto writeStatus =
      _provider->writeMessages(std::move(messages), ClockDurationNano::max());

  if (writeStatus == ChannelProvider::Result::PermanentFailure) {
    /*
     *  If the channel is unrecoverable, we need to get rid of our
     *  reference to the descriptor and warn the user of the failure.
     */
    terminate();
    return false;
  }

  return writeStatus == ChannelProvider::Result::Success;
}

const Channel::MessageCallback& Channel::messageCallback() const {
  return _messageCallback;
}

void Channel::setMessageCallback(MessageCallback callback) {
  _messageCallback = callback;
}

bool Channel::readPendingMessageNow() {
  auto result = _provider->readMessage(ClockDurationNano::max());

  switch (result.first) {
    case ChannelProvider::Result::Success:
      if (_messageCallback) {
        _messageCallback(std::move(result.second));
      }
      return true;
    case ChannelProvider::Result::PermanentFailure:
      terminate();
      return false;
    case ChannelProvider::Result::TemporaryFailure:
      return false;
  }

  return false;
}

Messages Channel::drainPendingMessages() {
  Messages messages;

  while (true) {
    auto result = _provider->readMessage(ClockDurationNano(0));

    if (result.first == ChannelProvider::Result::Success) {
      messages.emplace_back(std::move(result.second));
      continue;
    }

    if (result.first == ChannelProvider::Result::PermanentFailure) {
      terminate();
    }

    break;
  }

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
