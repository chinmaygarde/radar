// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef RADARLOVE_INTERFACE_INTERFACE_
#define RADARLOVE_INTERFACE_INTERFACE_

#include <Core/Core.h>

#include <Coordinator/CoordinatorAcquisitionProtocol.h>
#include <Interface/InterfaceDelegate.h>
#include <Interface/InterfaceTransaction.h>
#include <Interface/ModelEntity.h>
#include <Layout/Constraint.h>
#include <Toolbox/StateMachine.h>

#include <vector>
#include <mutex>

namespace rl {
namespace interface {

class Interface {
 public:
  enum State {
    /**
     *  The is not running and none of the delgate methods or callbacks will be
     *  hit
     */
    NotRunning,
    /**
     *  The interface is in the inactive state. This may be because it is not
     *  currently in focus.
     */
    Inactive,
    /**
     *  The interface is in the active state and in the foreground
     */
    Active,
    /**
     *  The interface is in background execution mode.
     */
    Background,
  };

  Interface(std::shared_ptr<InterfaceDelegate> delegate);

  Interface(std::shared_ptr<InterfaceDelegate> delegate,
            std::unique_ptr<core::Archive> spliceArchive);

  /**
   *  Setup the interface context and invoke the callback when ready
   *
   *  @param onReady the callback that gets invoked on readiness
   */
  void run(std::function<void()> onReady = nullptr);

  /**
   *  @return if the interface context running
   */
  bool isRunning() const;

  ModelEntity::Ref createEntity();

  /**
   *  Gracefully shutdown the interface
   *
   *  @param onShutdown the callback to invoke on shutdown
   */
  void shutdown(std::function<void()> onShutdown = nullptr);

  ModelEntity& rootEntity();

  /**
   *  Get the current size of the interface
   *
   *  @return the current interface size
   */
  const geom::Size& size() const;

  /**
   *  Get the transaction on top of the interface transaction stack
   *
   *  @return the top transaction
   */
  InterfaceTransaction& transaction();

  /**
   *  Push a new transaction onto the transaction stack
   *
   *  @param action the action for the transaction
   */
  void pushTransaction(animation::Action&& action);

  /**
   *  Pop the last transaction off the transaction stack
   */
  void popTransaction();

  State state() const;

  void setupConstraints(const std::vector<layout::Constraint>& constraints);

  void setupConstraintSuggestions(
      const std::vector<layout::Suggestion>& suggestions);

 protected:
  virtual void didBecomeReady();

  core::Namespace& ns();

 private:
  std::string _remoteDebugTag;
  core::Namespace _localNS;
  ModelEntity _rootEntity;
  core::EventLoop* _loop;
  std::mutex _transactionStackMutex;
  std::vector<std::unique_ptr<InterfaceTransaction>> _transactionStack;
  std::vector<std::unique_ptr<InterfaceTransaction>> _committedTransactions;
  std::shared_ptr<core::EventLoopObserver> _autoFlushObserver;
  std::shared_ptr<InterfaceDelegate> _delegate;
  std::shared_ptr<core::Channel> _coordinatorChannel;
  std::unique_ptr<core::Archive> _spliceArchive;
  toolbox::StateMachine _state;
  coordinator::CoordinatorAcquisitionProtocol _coordinatorAcquisition;

  void attemptCoordinatorChannelAcquisition();
  void onCoordinatorChannelAcquisition(core::IOResult result,
                                       core::Message message);
  void scheduleChannels();
  void unscheduleChannels();

  void autoFlushObserver(core::EventLoopObserver::Activity activity);
  void armAutoFlushTransactions(bool arm);
  void flushTransactions();
  bool sendTransactionMessage();
  void performTerminationCleanup();

  void entityDidRecordUpdateUpdate(const entity::Entity& entity,
                                   entity::Entity::Property property,
                                   core::Name otherIdentifier);

  void didFinishLaunching();
  void didBecomeActive();
  void didEnterBackground();
  void didTerminate();
  void didBecomeInactive();

  RL_DISALLOW_COPY_AND_ASSIGN(Interface);
};

}  // namespace interface
}  // namespace rl

#endif  // RADARLOVE_INTERFACE_INTERFACE_
