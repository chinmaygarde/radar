// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "Interface/Interface.h"

#include <limits>
#include <cassert>

namespace rl {

Interface::Interface() : _looper(nullptr), _lock(), _transactionStack() {
  _autoFlushObserver = std::make_shared<LooperObserver>(
      std::numeric_limits<uint64_t>::max(), [&] {
        flushTransactions();
        armAutoFlushTransactions(false);
      });
}

void Interface::run(Latch& readyLatch) {
  if (_looper != nullptr) {
    readyLatch.countDown();
    return;
  }

  _looper = Looper::Current();
  _looper->loop([&]() {
    /*
     *  Event channels might need to interact with the looper. So wait to setup
     *  channel connections till after the looper says its ready.
     */
    setupEventChannels();
    readyLatch.countDown();
  });
}

bool Interface::isRunning() const {
  return _looper != nullptr;
}

const InterfaceTransaction& Interface::transaction() {
  std::lock_guard<std::mutex> lock(_lock);

  if (_transactionStack.size() == 0) {
    /**
     *  If the transaction stack is empty, push the default transaction. We
     *  are already holding the lock, so update the stack manually.
     */
    _transactionStack.emplace();
    armAutoFlushTransactions(true);
  }

  return _transactionStack.top();
}

template <typename... T>
void Interface::pushTransaction(T&&... args) {
  std::lock_guard<std::mutex> lock(_lock);
  _transactionStack.emplace(args...);
}

void Interface::popTransaction() {
  std::lock_guard<std::mutex> lock(_lock);

  if (_transactionStack.size() == 0) {
    return;
  }

  _transactionStack.top().commit();
  _transactionStack.pop();
}

void Interface::armAutoFlushTransactions(bool arm) {
  const auto activity = LooperObserver::Activity::BeforeSleep;

  if (arm) {
    _looper->addObserver(_autoFlushObserver, activity);
  } else {
    _looper->removeObserver(_autoFlushObserver, activity);
  }
}

void Interface::flushTransactions() {
  std::lock_guard<std::mutex> lock(_lock);

  while (_transactionStack.size() != 0) {
    _transactionStack.top().commit();
    _transactionStack.pop();
  }
}

void Interface::setupEventChannels() {
  assert(_looper == Looper::Current());
  _eventsChannel = Channel::CreateConnectedChannels();
  bool result = _looper->addSource(_eventsChannel.second->source());
  assert(result == true);
}

std::shared_ptr<Channel> Interface::sendEventChannel() const {
  assert(_eventsChannel.first != nullptr);
  return _eventsChannel.first;
}

}  // namespace rl
