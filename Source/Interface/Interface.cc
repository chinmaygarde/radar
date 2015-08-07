// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "Interface/Interface.h"

#include <limits>
#include <cassert>

namespace rl {

Interface::Interface(std::weak_ptr<InterfaceDelegate> delegate)
    : _looper(nullptr),
      _lock(),
      _transactionStack(),
      _touchEventChannel(),
      _delegate(delegate),
      _state({
// clang-format off
          #define C(x) std::bind(&Interface::x, this)
          // From        | To          | Callback
          {  NotRunning,   Inactive,     C(didFinishLaunching)  },
          {  NotRunning,   Background,   C(didEnterBackground)  },
          {  Inactive,     NotRunning,   C(didTerminate)        },
          {  Inactive,     Active,       C(didBecomeActive)     },
          {  Active,       Inactive,     C(didBecomeInactive)   },
          {  Background,   NotRunning,   C(didTerminate)        },
          #undef F
          // clang-format on
      }) {
  _autoFlushObserver = std::make_shared<LooperObserver>(
      std::numeric_limits<uint64_t>::max(), [&] {
        flushTransactions();
        armAutoFlushTransactions(false);
      });

  ;
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
    _state.setState(Active, true);
  });
}

bool Interface::isRunning() const {
  return _looper != nullptr;
}

void Interface::shutdown(rl::Latch& onShutdown) {
  if (_looper == nullptr) {
    onShutdown.countDown();
    return;
  }

  _looper->dispatchAsync([&]() {
    performTerminationCleanup();
    cleanupEventChannels();
    _looper->terminate();
    onShutdown.countDown();
  });
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
  bool result = _looper->addSource(_touchEventChannel.source());
  assert(result == true);
}

void Interface::cleanupEventChannels() {
  bool result = _looper->removeSource(_touchEventChannel.source());
  assert(result == true);
}

TouchEventChannel& Interface::touchEventChannel() {
  return _touchEventChannel;
}

Interface::State Interface::state() const {
  return static_cast<Interface::State>(_state.state());
}

void Interface::didFinishLaunching() {
  if (auto delegate = _delegate.lock()) {
    delegate->didFinishLaunching(*this);
  }
}

void Interface::didBecomeActive() {
  if (auto delegate = _delegate.lock()) {
    delegate->didBecomeActive(*this);
  }
}

void Interface::didBecomeInactive() {
  if (auto delegate = _delegate.lock()) {
    delegate->didBecomeInactive(*this);
  }
}

void Interface::didEnterBackground() {
  if (auto delegate = _delegate.lock()) {
    delegate->didEnterBackground(*this);
  }
}

void Interface::didTerminate() {
  if (auto delegate = _delegate.lock()) {
    delegate->didTerminate(*this);
  }
}

void Interface::performTerminationCleanup() {
  _state.setState(NotRunning, true);
}

}  // namespace rl
