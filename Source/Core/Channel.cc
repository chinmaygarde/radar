// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "Core/Channel.h"
#include "Core/Message.h"
#include "Core/Utilities.h"
#include "Core/Channel/SocketChannel.h"
#include "Core/Channel/MachPortChannel.h"

namespace rl {

Channel::ConnectedPair Channel::CreateConnectedPair() {
#if __APPLE__
  return MachPortChannel::CreateConnectedPair();
#else
  return SocketChannel::CreateConnectedPair();
#endif
}

Channel::Channel(std::string name)
    : _terminated(false), _connected(false), _name(name) {
}

Channel::~Channel() {
  terminate();
}

bool Channel::connect() {
  if (_terminated) {
    return false;
  }

  if (_connected) {
    return true;
  }

  _connected = doConnect(_name);

  return _connected;
}

bool Channel::isConnected() const {
  return _connected;
}

void Channel::terminate() {
  if (_terminated) {
    assert(_connected == false);
    return;
  }

  bool closed = doTerminate();

  _connected = false;
  _terminated = true;

  if (closed && _terminationCallback) {
    _terminationCallback();
  }
}

bool Channel::sendMessage(Message& message) {
  RL_ASSERT(message.size() <= 1024);

  auto writeStatus = WriteMessage(message);

  if (writeStatus == Channel::Result::PermanentFailure) {
    /*
     *  If the channel is unrecoverable, we need to get rid of our
     *  reference to the descriptor and warn the user of the failure.
     */
    terminate();
    return false;
  }

  return writeStatus == Channel::Result::Success;
}

const Channel::MessageReceivedCallback& Channel::messageReceivedCallback()
    const {
  return _messageReceivedCallback;
}

void Channel::setMessageReceivedCallback(MessageReceivedCallback callback) {
  _messageReceivedCallback = callback;
}

void Channel::readPendingMessageNow() {
  Channel::Result status;
  std::vector<std::unique_ptr<Message>> messages;

  std::tie(status, messages) = ReadMessages();

  /*
   *  Dispatch all successfully read messages
   */
  if (_messageReceivedCallback) {
    for (auto& message : messages) {
      _messageReceivedCallback(*message.get());
    }
  }

  /*
   *  On fatal errors, terminate the channel
   */
  if (status == Channel::Result::PermanentFailure) {
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

Message Channel::createMessage(size_t reserved) const {
  return Message(reserved);
}

}  // namespace rl
