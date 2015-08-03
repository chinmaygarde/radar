// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "Core/Channel.h"
#include "Core/Message.h"
#include "Core/Utilities.h"

namespace rl {

Channel::Channel(std::string name) : _connected(false), _name(name) {
  _socket = Utils::make_unique<Socket>();
  _ready = true;
}

Channel::Channel(Handle handle) : _ready(true), _connected(true) {
  _socket = Utils::make_unique<Socket>(handle);
}

Channel::Channel(std::unique_ptr<Socket> socket)
    : _socket(std::move(socket)), _ready(true), _connected(true) {
}

Channel::ConnectedChannels Channel::CreateConnectedChannels() {
  auto socketPair = Socket::CreatePair();

  return ConnectedChannels(
      std::make_shared<Channel>(std::move(socketPair.first)),
      std::make_shared<Channel>(std::move(socketPair.second)));
}

void Channel::terminate() {
  bool closed = _socket->close();

  _connected = false;
  _ready = false;

  if (closed && _terminationCallback) {
    _terminationCallback();
  }
}

Channel::~Channel() {
  terminate();
}

bool Channel::tryConnect() {
  if (_connected) {
    return true;
  }

  _connected = _socket->connect(_name);

  return _connected;
}

bool Channel::isConnected() const {
  return _connected;
}

bool Channel::isReady() const {
  return _ready;
}

std::shared_ptr<LooperSource> Channel::source() {
  if (_source.get() != nullptr) {
    return _source;
  }

  RL_ASSERT(_connected == true);

  Handle handle = _socket->handle();

  using LS = LooperSource;

  LS::IOHandlesAllocator allocator = [handle]() {
    return LS::Handles(handle, handle); /* bi-di connection */
  };

  LS::IOHandler readHandler =
      [this](LS::Handle handle) { this->readMessageOnHandle(handle); };

  /**
   *  We are specifying a null write handler since we will
   *  never directly signal this source. Instead, we will write
   *  to the handle directly.
   *
   *  The channel own the socket handle, so there is no deallocation
   *  callback either.
   */
  _source = std::make_shared<LS>(allocator, nullptr, readHandler, nullptr);

  return _source;
}

bool Channel::sendMessage(Message& message) {
  RL_ASSERT(message.size() <= 1024);

  auto writeStatus = _socket->WriteMessage(message);

  if (writeStatus == Socket::Result::PermanentFailure) {
    /*
     *  If the channel is unrecoverable, we need to get rid of our
     *  reference to the descriptor and warn the user of the failure.
     */
    terminate();
    return false;
  }

  return writeStatus == Socket::Result::Success;
}

const Channel::MessageReceivedCallback& Channel::messageReceivedCallback()
    const {
  return _messageReceivedCallback;
}

void Channel::setMessageReceivedCallback(MessageReceivedCallback callback) {
  _messageReceivedCallback = callback;
}

void Channel::readMessageOnHandle(Handle handle) {
  Socket::Result status;
  std::vector<std::unique_ptr<Message>> messages;

  std::tie(status, messages) = _socket->ReadMessages();

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
  if (status == Socket::Result::PermanentFailure) {
    terminate();
    return;
  }
}

void Channel::scheduleInLooper(Looper* looper) {
  if (looper == nullptr || !_connected) {
    return;
  }

  looper->addSource(source());
}

void Channel::unscheduleFromLooper(Looper* looper) {
  if (looper == nullptr) {
    return;
  }

  /*
   * don't invoke the accessor which implicitly constructs a source
   */
  looper->removeSource(_source);
}

Channel::TerminationCallback Channel::terminationCallback() const {
  return _terminationCallback;
}

/**
 *  Set the termination callback
 *
 *  @param callback the termination callback
 */
void Channel::setTerminationCallback(Channel::TerminationCallback callback) {
  _terminationCallback = callback;
}

}  // namespace rl
