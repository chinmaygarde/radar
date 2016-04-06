// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <Coordinator/Coordinator.h>
#include <Core/ThreadLocal.h>
#include <Core/TraceEvent.h>
#include <Interface/Interface.h>

#include <limits>

namespace rl {
namespace interface {

using LT = toolbox::StateMachine::LegalTransition;

Interface::Interface(std::weak_ptr<InterfaceDelegate> delegate)
    : Interface(delegate, nullptr) {}

Interface::Interface(std::weak_ptr<InterfaceDelegate> delegate,
                     std::unique_ptr<core::Archive> spliceArchive)
    : _localNS(),
      _rootEntity(_localNS,
                  std::bind(&Interface::entityDidRecordUpdateUpdate,
                            this,
                            std::placeholders::_1,
                            std::placeholders::_2,
                            std::placeholders::_3)),
      _loop(nullptr),
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
  RL_ASSERT_MSG(_delegate.lock() != nullptr,
                "A valid delegate must be present");

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
    scheduleChannels();
    _state.setState(Active, true);
    attemptCoordinatorChannelAcquisition();
    transaction().mark(_rootEntity, entity::Entity::Property::MakeRoot,
                       core::Name{});
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

core::Namespace& Interface::ns() {
  return _localNS;
}

std::unique_ptr<ModelEntity> Interface::createEntity() {
  ModelEntity::UpdateCallback callback = std::bind(
      &Interface::entityDidRecordUpdateUpdate, this, std::placeholders::_1,
      std::placeholders::_2, std::placeholders::_3);
  return core::make_unique<ModelEntity>(core::Name{_localNS}, callback);
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
    if (onShutdown) {
      onShutdown();
    }
  });
}

void Interface::attemptCoordinatorChannelAcquisition() {
  _coordinatorAcquisition.sendRequest(
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

void Interface::entityDidRecordUpdateUpdate(const entity::Entity& entity,
                                            entity::Entity::Property property,
                                            core::Name otherIdentifier) {
  transaction().mark(entity, property, otherIdentifier);
}

InterfaceTransaction& Interface::transaction() {
  std::lock_guard<std::mutex> lock(_transactionStackMutex);

  if (_transactionStack.size() == 0) {
    /*
     *  If the transaction stack is empty, push the default transaction. We
     *  are already holding the lock, so update the stack manually.
     */
    _transactionStack.emplace_back(
        core::make_unique<InterfaceTransaction>(animation::Action{0.0}));
    armAutoFlushTransactions(true);
  }

  return *_transactionStack.back();
}

void Interface::pushTransaction(animation::Action&& action) {
  std::lock_guard<std::mutex> lock(_transactionStackMutex);
  _transactionStack.emplace_back(
      core::make_unique<InterfaceTransaction>(std::move(action)));
}

void Interface::popTransaction() {
  std::lock_guard<std::mutex> lock(_transactionStackMutex);

  if (_transactionStack.size() == 0) {
    return;
  }

  _committedTransactions.emplace_back(std::move(_transactionStack.back()));
  _transactionStack.pop_back();
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

  auto result = true;

  /*
   *  Create a message to encode all the transaction items into
   */
  core::Message arena;

  std::lock_guard<std::mutex> lock(_transactionStackMutex);

  /*
   *  Commit all explicitly committed transactions.
   */
  for (auto& transaction : _committedTransactions) {
    result &= transaction->commit(arena, _spliceArchive);
  }

  /*
   *  Commit the transactions still pending on the transaction stack.
   */
  for (auto i = _transactionStack.rbegin(), end = _transactionStack.rend();
       i != end; i++) {
    (*i)->commit(arena, _spliceArchive);
  }

  core::Messages messages;
  messages.push_back(std::move(arena));

  result = _coordinatorChannel->sendMessages(std::move(messages)) ==
           core::IOResult::Success;

  _committedTransactions.clear();
  _transactionStack.clear();

  RL_ASSERT_MSG(result, "Must be able to flush the coordinator transaction");
}

void Interface::scheduleChannels() {
  RL_ASSERT(_loop == core::EventLoop::Current());
  /*
   *  The event loop is ready, schedule all event channels the interface cares
   *  about
   */
  _loop->addSource(_coordinatorAcquisition.source());
}

void Interface::unscheduleChannels() {
  /*
   *  The event loop is about to die, unschedule all active channels
   */
  _loop->removeSource(_coordinatorAcquisition.source());
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
