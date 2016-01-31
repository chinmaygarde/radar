// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <Coordinator/Coordinator.h>
#include <Core/ThreadLocal.h>
#include <Instrumentation/TraceEvent.h>
#include <Interface/Interface.h>

#include <limits>

namespace rl {
namespace interface {

RL_THREAD_LOCAL core::ThreadLocal CurrentInterface;

using LT = toolbox::StateMachine::LegalTransition;

Interface::Interface(std::weak_ptr<InterfaceDelegate> delegate)
    : Interface(delegate, nullptr) {}

Interface::Interface(std::weak_ptr<InterfaceDelegate> delegate,
                     std::unique_ptr<core::Archive>
                         spliceArchive)
    : _localNS(),
      _rootEntity(_localNS),
      _loop(nullptr),
      _popCount(0),
      _delegate(delegate),
      _spliceArchive(std::move(spliceArchive)),
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
  /*
   *  Implicit interface transactions are flushed at the maximum available
   *  priority. This is so that loop observers setup by application code can
   *  be serviced before the implicit flush.
   */
  _autoFlushObserver = std::make_shared<core::EventLoopObserver>(
      std::bind(&Interface::autoFlushObserver, this, std::placeholders::_1),
      std::numeric_limits<int64_t>::max());
}

void Interface::run(std::function<void()> onReady) {
  if (_loop != nullptr) {
    if (onReady) {
      onReady();
    }
    return;
  }

  _loop = core::EventLoop::Current();
  _loop->loop([&]() {
    CurrentInterface.set(reinterpret_cast<uintptr_t>(this));
    scheduleChannels();
    _state.setState(Active, true);
    attemptCoordinatorChannelAcquisition();
    transaction().mark(_rootEntity, Entity::Property::MakeRoot, core::Name{});
    if (onReady) {
      onReady();
    }
    didBecomeReady();
  });
}

void Interface::didBecomeReady() {
  // Base class implementation does nothing.
}

bool Interface::isRunning() const {
  return _loop != nullptr;
}

std::unique_ptr<ModelEntity> Interface::createEntity() {
  return core::make_unique<ModelEntity>(core::Name{_localNS});
}

void Interface::shutdown(std::function<void()> onShutdown) {
  if (_loop == nullptr) {
    if (onShutdown) {
      onShutdown();
    }
    return;
  }

  _loop->dispatchAsync([&]() {
    performTerminationCleanup();
    unscheduleChannels();
    _loop->terminate();
    CurrentInterface.set(reinterpret_cast<uintptr_t>(nullptr));
    if (onShutdown) {
      onShutdown();
    }
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

void Interface::attemptCoordinatorChannelAcquisition() {
  _interfaceAcquisition.sendRequest(
      std::bind(&Interface::onCoordinatorChannelAcquisition, this,
                std::placeholders::_1, std::placeholders::_2));
}

void Interface::onCoordinatorChannelAcquisition(core::IOResult result,
                                                core::Message message) {
  if (result != core::IOResult::Success) {
    return;
  }

  if (message.attachments().size() != 1) {
    return;
  }

  /*
   *  Channel acqusition messages contain one attachment with the channel handle
   */
  _coordinatorChannel = std::make_shared<core::Channel>(
      core::Message::Attachment{message.attachments()[0]});

  /*
   *  The message contains one string containing the debug tag
   */
  bool tagRead = message.decode(_remoteDebugTag);

  RL_ASSERT(tagRead && message.readCompleted());

  if (!isRunning()) {
    return;
  }

  _loop->dispatchAsync([&] { flushTransactions(); });
}

InterfaceTransaction& Interface::transaction() {
  std::lock_guard<std::mutex> lock(_lock);

  if (_transactionStack.size() == 0) {
    /*
     *  If the transaction stack is empty, push the default transaction. We
     *  are already holding the lock, so update the stack manually.
     */
    _transactionStack.emplace_back(Action(0.0));
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

void Interface::autoFlushObserver(core::EventLoopObserver::Activity activity) {
  RL_ASSERT(activity == core::EventLoopObserver::Activity::BeforeSleep);
  flushTransactions();
  armAutoFlushTransactions(false);
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
  if (_transactionStack.size() == 0 || _coordinatorChannel == nullptr) {
    return;
  }

  RL_TRACE_AUTO("Interface::FlushTransactions")
  RL_ASSERT(_coordinatorChannel != nullptr);

  std::lock_guard<std::mutex> lock(_lock);

  auto result = true;

  /*
   *  Create a message to encode all the transaction items into
   */
  core::Message arena;

  for (auto& transaction : _transactionStack) {
    result &= transaction.commit(arena, _spliceArchive);
  }

  core::Messages messages;
  messages.push_back(std::move(arena));

  result = _coordinatorChannel->sendMessages(std::move(messages)) ==
           core::IOResult::Success;

  _popCount = 0;
  _transactionStack.clear();

  RL_ASSERT_MSG(result, "Must be able to flush the coordinator transaction");
}

void Interface::scheduleChannels() {
  RL_ASSERT(_loop == core::EventLoop::Current());
  /*
   *  The event loop is ready, schedule all event channels the interface cares
   *  about
   */
  _loop->addSource(_interfaceAcquisition.source());
}

void Interface::unscheduleChannels() {
  /*
   *  The event loop is about to die, unschedule all active channels
   */
  _loop->removeSource(_interfaceAcquisition.source());
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
  if (auto delegate = _delegate.lock()) {
    delegate->didTerminate(*this);
  }
}

void Interface::didUpdateSize() {
  if (auto delegate = _delegate.lock()) {
    delegate->didUpdateSize(*this);
  }

  _rootEntity.setFrame({{0.0, 0.0}, size()});
  setupConstraintSuggestions(
      layout::Suggestion::Anchor(_rootEntity, layout::priority::Strong));
}

void Interface::performTerminationCleanup() {
  _state.setState(NotRunning, true);
}

ModelEntity& Interface::rootEntity() {
  return _rootEntity;
}

void Interface::setupConstraints(
    const std::vector<layout::Constraint>& constraints) {
  transaction().mark(constraints);
}

void Interface::setupConstraintSuggestions(
    const std::vector<layout::Suggestion>& suggestions) {
  transaction().mark(suggestions);
}

}  // namespace interface
}  // namespace rl
