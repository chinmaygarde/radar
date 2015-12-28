// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef RADARLOVE_INTERFACE_INTERFACE_
#define RADARLOVE_INTERFACE_INTERFACE_

#include <Core/Core.h>
#include <Interface/InterfaceDelegate.h>
#include <Interface/InterfaceTransaction.h>
#include <Interface/Layer.h>
#include <Layout/Constraint.h>
#include <Toolbox/StateMachine.h>

#include <deque>
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

  explicit Interface(std::weak_ptr<InterfaceDelegate> delegate);

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

  /**
   *  Gracefully shutdown the interface
   *
   *  @return onShutdown the callback to invoke on shutdown
   */
  void shutdown(std::function<void()> onShutdown = nullptr);

  /**
   *  Return the current root layer for the interface
   *
   *  @return the root layer for the interface
   */
  const Layer::Ref rootLayer() const;

  /**
   *  Set the current root layer for the interface
   *
   *  @param layer the new root layer
   */
  void setRootLayer(Layer::Ref layer);

  /**
   *  Get the current size of the interface
   *
   *  @return the current interface size
   */
  const geom::Size& size() const;

  /**
   *  Update the size of the interface
   *
   *  @param size the new interface size
   */
  void setSize(const geom::Size& size);

  /**
   *  Get the transaction on top of the interface transaction stack
   *
   *  @return the top transaction
   */
  InterfaceTransaction& transaction();

  /**
   *  Push a new transaction onto the transaction stack
   *
   *  @param args the action for the transaction
   */
  void pushTransaction(Action&& action);

  /**
   *  Pop the last transaction off the transaction stack
   */
  void popTransaction();

  State state() const;

  /**
   *  If the current thread has an interface attached to it, that interface is
   *  returned. In not returns nullptr.
   *
   *  @return the current interface if one is present
   */
  static Interface& current();

  void setupConstraints(const std::vector<layout::Constraint>& constraints);

  void setupConstraintSuggestions(
      const std::vector<layout::Suggestion>& suggestions);

 private:
  core::EventLoop* _loop;
  geom::Size _size;
  std::mutex _lock;
  Layer::Ref _rootLayer;
  std::deque<InterfaceTransaction> _transactionStack;
  size_t _popCount;
  std::shared_ptr<core::EventLoopObserver> _autoFlushObserver;
  std::weak_ptr<InterfaceDelegate> _delegate;
  std::shared_ptr<core::Channel> _coordinatorChannel;
  toolbox::StateMachine _state;

  void coordinatorChannelAcquired(std::shared_ptr<core::Channel> channel);
  void scheduleChannels();
  void unscheduleChannels();

  void autoFlushObserver(core::EventLoopObserver::Activity activity);
  void armAutoFlushTransactions(bool arm);
  void flushTransactions();
  bool sendTransactionMessage();
  void performTerminationCleanup();

  void didFinishLaunching();
  void didBecomeActive();
  void didEnterBackground();
  void didTerminate();
  void didBecomeInactive();
  void didUpdateSize();

  RL_DISALLOW_COPY_AND_ASSIGN(Interface);
};

}  // namespace interface
}  // namespace rl

#endif  // RADARLOVE_INTERFACE_INTERFACE_
