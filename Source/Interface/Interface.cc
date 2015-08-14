// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <Interface/Interface.h>

#include <limits>
#include <cassert>
#include <pthread.h>

namespace rl {

static pthread_key_t InterfaceTLSKey() {
  static std::once_flag once;
  static pthread_key_t interfaceKey;
  std::call_once(once, []() { pthread_key_create(&interfaceKey, nullptr); });
  return interfaceKey;
}

Interface::Interface(std::weak_ptr<InterfaceDelegate> delegate,
                     InterfaceLease& lease)
    : _looper(nullptr),
      _size(0.0, 0.0),
      _lock(),
      _rootLayer(nullptr),
      _transactionStack(),
      _touchEventChannel(),
      _delegate(delegate),
      _lease(lease),
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
    pthread_setspecific(InterfaceTLSKey(), this);
    setupEventChannels();
    _state.setState(Active, true);
    readyLatch.countDown();
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
    pthread_setspecific(InterfaceTLSKey(), nullptr);
    onShutdown.countDown();
  });
}

const Size& Interface::size() const {
  return _size;
}

void Interface::setSize(const Size& size) {
  if (_size == size) {
    return;
  }

  _size = size;

  if (isRunning()) {
    _looper->dispatchAsync([&]() { didUpdateSize(); });
  }
}

InterfaceTransaction& Interface::transaction() {
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

  finalizeLeaseWrite();
}

void Interface::finalizeLeaseWrite() {
  if (_rootLayer == nullptr) {
    return;
  }

  /*
   *  Serialize entities to the current write arena in rendering order
   */
  std::stack<Layer::Ref> stack;
  stack.push(_rootLayer);

  auto writeArena = _lease.accessWriteArena(false, false);

  while (stack.size() > 0) {
    auto current = stack.top();
    writeArena.emplacePresentationEntity(*current);
    stack.pop();

    auto& sublayers = current->sublayers();
    for (auto i = sublayers.rbegin(), end = sublayers.rend(); i != end; ++i) {
      stack.push(*i);
    }
  }

  _lease.accessWriteArena(true, true);
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

Interface& Interface::current() {
  auto interface =
      reinterpret_cast<Interface*>(pthread_getspecific(InterfaceTLSKey()));

  assert(interface != nullptr &&
         "Layer modification on a non-interface threads is forbidden");

  return *interface;
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
  _rootLayer = nullptr;

  if (auto delegate = _delegate.lock()) {
    delegate->didTerminate(*this);
  }
}

void Interface::didUpdateSize() {
  if (auto delegate = _delegate.lock()) {
    delegate->didUpdateSize(*this);
  }

  if (_rootLayer) {
    _rootLayer->setFrame({{0.0, 0.0}, size()});
  }
}

void Interface::performTerminationCleanup() {
  _state.setState(NotRunning, true);
}

const Layer::Ref Interface::rootLayer() const {
  return _rootLayer;
}

void Interface::setRootLayer(Layer::Ref layer) {
  _rootLayer = layer;
}

}  // namespace rl
