// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <Core/ThreadLocal.h>
#include <Instrumentation/TraceEvent.h>
#include <Interface/Interface.h>

#include <limits>

namespace rl {
namespace interface {

RL_THREAD_LOCAL core::ThreadLocal CurrentInterface;

using LT = toolbox::StateMachine::LegalTransition;

Interface::Interface(std::weak_ptr<InterfaceDelegate> delegate,
                     std::weak_ptr<coordinator::Channel>
                         compositorChannel)
    : _loop(nullptr),
      _size(0.0, 0.0),
      _lock(),
      _rootLayer(nullptr),
      _transactionStack(),
      _popCount(0),
      _delegate(delegate),
      _compositorChannel(compositorChannel),
      _state({
// clang-format off
          #define C(x) std::bind(&Interface::x, this)
          //    From        | To          | Callback
          LT {  NotRunning,   Inactive,     C(didFinishLaunching)  },
          LT {  NotRunning,   Background,   C(didEnterBackground)  },
          LT {  Inactive,     NotRunning,   C(didTerminate)        },
          LT {  Inactive,     Active,       C(didBecomeActive)     },
          LT {  Active,       Inactive,     C(didBecomeInactive)   },
          LT {  Background,   NotRunning,   C(didTerminate)        },
          #undef C
          // clang-format on
      }) {
  _autoFlushObserver = std::make_shared<core::EventLoopObserver>(
      std::numeric_limits<uint64_t>::max(), [&] {
        flushTransactions();
        armAutoFlushTransactions(false);
      });
}

void Interface::run(core::Latch& readyLatch) {
  if (_loop != nullptr) {
    readyLatch.countDown();
    return;
  }

  _loop = core::EventLoop::Current();
  _loop->loop([&]() {
    CurrentInterface.set(reinterpret_cast<uintptr_t>(this));
    scheduleChannels();
    _state.setState(Active, true);
    readyLatch.countDown();
  });
}

bool Interface::isRunning() const {
  return _loop != nullptr;
}

void Interface::shutdown(core::Latch& onShutdown) {
  if (_loop == nullptr) {
    onShutdown.countDown();
    return;
  }

  _loop->dispatchAsync([&]() {
    performTerminationCleanup();
    unscheduleChannels();
    _loop->terminate();
    CurrentInterface.set(reinterpret_cast<uintptr_t>(nullptr));
    onShutdown.countDown();
  });
}

const geom::Size& Interface::size() const {
  return _size;
}

void Interface::setSize(const geom::Size& size) {
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
    /*
     *  If the transaction stack is empty, push the default transaction. We
     *  are already holding the lock, so update the stack manually.
     */
    _transactionStack.emplace_back(std::move(Action(0.0)));
    armAutoFlushTransactions(true);
  }

  return _transactionStack[_transactionStack.size() - _popCount - 1];
}

void Interface::pushTransaction(Action&& action) {
  std::lock_guard<std::mutex> lock(_lock);
  _transactionStack.emplace_back(std::move(action));
}

void Interface::popTransaction() {
  std::lock_guard<std::mutex> lock(_lock);

  if (_transactionStack.size() == 0) {
    return;
  }

  _popCount++;
}

void Interface::armAutoFlushTransactions(bool arm) {
  const auto activity = core::EventLoopObserver::Activity::BeforeSleep;

  if (arm) {
    _loop->addObserver(_autoFlushObserver, activity);
  } else {
    _loop->removeObserver(_autoFlushObserver, activity);
  }
}

void Interface::flushTransactions() {
  RL_TRACE_AUTO("Interface::FlushTransactions")

  std::lock_guard<std::mutex> lock(_lock);

  auto compositor = _compositorChannel.lock();
  RL_ASSERT_MSG(compositor, "Compositor channel must be present");
  auto& arena = compositor->transactionMessage();

  bool result = true;

  for (auto& transaction : _transactionStack) {
    result &= transaction.commit(arena);
  }

  result &= compositor->flushTransaction();

  _popCount = 0;
  _transactionStack.clear();

  RL_ASSERT_MSG(result, "Must be able to flush the compositor transaction");
}

void Interface::scheduleChannels() {
  RL_ASSERT(_loop == core::EventLoop::Current());
  /*
   *  The event loop is ready, schedule all event channels the interface cares
   *  about
   */
}

void Interface::unscheduleChannels() {
  /*
   *  The event loop is about to die, unschedule all active channels
   */
}

Interface::State Interface::state() const {
  return static_cast<Interface::State>(_state.state());
}

Interface& Interface::current() {
  auto interface = reinterpret_cast<Interface*>(CurrentInterface.get());
  RL_ASSERT_MSG(interface != nullptr,
                "Layer modification on non-interface threads is forbidden");
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
    setupConstraints(layout::Constraint::AnchorConstraints(*_rootLayer));
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

  if (layer) {
    layer->makeRootInInterface();
  }
}

bool Interface::setupGestureRecognizer(
    rl::recognition::GestureRecognizer&& recognizer) {
  if (!recognizer.isSolvable()) {
    return false;
  }
  transaction().mark(std::move(recognizer));
  return true;
}

void Interface::setupConstraints(
    const std::vector<layout::Constraint>& constraints) {
  transaction().mark(constraints);
}

}  // namespace interface
}  // namespace rl
