// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <Interface/Interface.h>

#include <limits>
#include <pthread.h>

namespace rl {

static pthread_key_t InterfaceTLSKey() {
  static std::once_flag once;
  static pthread_key_t interfaceKey;
  std::call_once(once, []() { pthread_key_create(&interfaceKey, nullptr); });
  return interfaceKey;
}

Interface::Interface(std::weak_ptr<InterfaceDelegate> delegate,
                     EntityLease& lease)
    : _loop(nullptr),
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
  _autoFlushObserver = std::make_shared<EventLoopObserver>(
      std::numeric_limits<uint64_t>::max(), [&] {
        flushTransactions();
        armAutoFlushTransactions(false);
      });

  ;
}

void Interface::run(Latch& readyLatch) {
  if (_loop != nullptr) {
    readyLatch.countDown();
    return;
  }

  _loop = EventLoop::Current();
  _loop->loop([&]() {
    pthread_setspecific(InterfaceTLSKey(), this);
    setupEventChannels();
    _state.setState(Active, true);
    readyLatch.countDown();
  });
}

bool Interface::isRunning() const {
  return _loop != nullptr;
}

void Interface::shutdown(rl::Latch& onShutdown) {
  if (_loop == nullptr) {
    onShutdown.countDown();
    return;
  }

  _loop->dispatchAsync([&]() {
    performTerminationCleanup();
    cleanupEventChannels();
    _loop->terminate();
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
    _loop->dispatchAsync([&]() { didUpdateSize(); });
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
  const auto activity = EventLoopObserver::Activity::BeforeSleep;

  if (arm) {
    _loop->addObserver(_autoFlushObserver, activity);
  } else {
    _loop->removeObserver(_autoFlushObserver, activity);
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
  using LayerDepth = std::pair<Layer::Ref, int32_t>;
  std::stack<LayerDepth> stack;
  stack.push(LayerDepth(_rootLayer, 0));

  auto writeArena = _lease.accessWriteArena(false, false);

  while (stack.size() > 0) {
    auto current = stack.top();

    writeArena.emplaceEntity(*(current.first), current.second);
    stack.pop();

    auto& sublayers = (current.first)->sublayers();
    for (auto i = sublayers.rbegin(), end = sublayers.rend(); i != end; ++i) {
      stack.push(LayerDepth(*i, 0));
    }
  }

  _lease.accessWriteArena(true, true);
}

void Interface::setupEventChannels() {
  RL_ASSERT(_loop == EventLoop::Current());
  bool result = _loop->addSource(_touchEventChannel.source());
  RL_ASSERT(result == true);
}

void Interface::cleanupEventChannels() {
  bool result = _loop->removeSource(_touchEventChannel.source());
  RL_ASSERT(result == true);
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

  RL_ASSERT(interface != nullptr &&
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
