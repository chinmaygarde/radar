// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <Core/Channel.h>
#include <Core/Message.h>
#include <Core/Utilities.h>
#include <Core/Channel/SocketChannel.h>
#include <Core/Channel/MachPortChannel.h>

namespace rl {

Channel::Channel()
    : _messagesReceivedCallback(nullptr),
      _terminationCallback(nullptr),
      _terminated(false),
      _provider(nullptr),
      _source(nullptr) {
#if __APPLE__
  _provider = make_unique<MachPortChannel>(*this);
#else
  _provider = make_unique<SocketChannel>(*this);
#endif
}

Channel::~Channel() {
  terminate();
}

void Channel::terminate() {
  if (_terminated) {
    return;
  }

  bool closed = _provider->doTerminate();

  _terminated = true;

  if (closed && _terminationCallback) {
    _terminationCallback();
  }
}

bool Channel::sendMessages(Messages messages) {
  if (messages.size() == 0) {
    return true;
  }

  auto writeStatus = _provider->WriteMessages(std::move(messages));

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

const Channel::MessagesReceivedCallback& Channel::messagesReceivedCallback()
    const {
  return _messagesReceivedCallback;
}

void Channel::setMessagesReceivedCallback(MessagesReceivedCallback callback) {
  _messagesReceivedCallback = callback;
}

void Channel::readPendingMessageNow() {
  ChannelProvider::Result status;
  Messages messages;

  std::tie(status, messages) = _provider->ReadMessages();

  /*
   *  Dispatch all successfully read messages
   */
  if (_messagesReceivedCallback) {
    _messagesReceivedCallback(std::move(messages));
  }

  /*
   *  On fatal errors, terminate the channel
   */
  if (status == ChannelProvider::Result::PermanentFailure) {
    terminate();
    return;
  }
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

}  // namespace rl
