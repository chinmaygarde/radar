// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "Interface/Interface.h"

#include <limits>

using namespace rl;

Interface::Interface() : _looper(nullptr), _lock(), _transactionStack() {
  _autoFlushObserver = std::make_shared<LooperObserver>(
      std::numeric_limits<uint64_t>::max(), [&] {
        armAutoFlushTransactions(false);
        flushTransactions();
      });
}

void Interface::run(Latch& readyLatch) {
  auto ready = [&readyLatch]() { readyLatch.countDown(); };

  if (_looper != nullptr) {
    ready();
    return;
  }

  _looper = Looper::Current();
  _looper->loop(ready);
}

bool Interface::isRunning() const {
  return _looper != nullptr;
}

const InterfaceTransaction& Interface::transaction() {
  AutoLock lock(_lock);

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
  AutoLock lock(_lock);
  _transactionStack.emplace(args...);
}

void Interface::popTransaction() {
  AutoLock lock(_lock);

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
}
