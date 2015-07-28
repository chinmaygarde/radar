// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "Channel.h"
#include "Message.h"
#include "Utilities.h"

using namespace rl;

Channel::Channel(std::string name) : _connected(false), _name(name) {
  _socket = Socket::Create();
  _ready = true;
}

Channel::Channel(Handle handle) : _ready(true), _connected(true) {
  _socket = Socket::Create(handle);
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

  Socket::Status writeStatus = _socket->WriteMessage(message);

  if (writeStatus == Socket::Status::PermanentFailure) {
    /*
     *  If the channel is unrecoverable, we need to get rid of our
     *  reference to the descriptor and warn the user of the failure.
     */
    terminate();
    return false;
  }

  return writeStatus == Socket::Status::Success;
}

void Channel::readMessageOnHandle(Handle handle) {
  Socket::Status status;
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
  if (status == Socket::Status::PermanentFailure) {
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

  /* don't invoke the accessor which implicitly constructs a
   source */
  looper->removeSource(_source);
}
