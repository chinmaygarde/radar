// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef RADARLOVE_INTERFACE_INTERFACE_
#define RADARLOVE_INTERFACE_INTERFACE_

#include <Core/Core.h>
#include <Interface/InterfaceTransaction.h>
#include <Interface/InterfaceDelegate.h>
#include <Toolbox/StateMachine.h>
#include <Interface/Layer.h>
#include <Coordinator/CompositorChannel.h>

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

  explicit Interface(
      std::weak_ptr<InterfaceDelegate> delegate,
      std::weak_ptr<coordinator::CompositorChannel> compositorChannel);

#pragma mark - Bootstrapping the interface

  /**
   *  Setup the interface context and count down on the latch when ready
   *
   *  @param readyLatch the latch to count down on readiness
   */
  void run(core::Latch& onReady);

  /**
   *  @return if the interface context running
   */
  bool isRunning() const;

  /**
   *  Gracefully shutdown the interface
   */
  void shutdown(core::Latch& onShutdown);

#pragma mark - Managing the Root Layer

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

#pragma mark - Changing Interface Properties

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

#pragma mark - Transaction Management

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

#pragma mark - Accessing State Information

  State state() const;

#pragma mark - Accessing the thread local interface

  /**
   *  If the current thread has an interface attached to it, that interface is
   *  returned. In not returns nullptr.
   *
   *  @return the current interface if one is present
   */
  static Interface& current();

#pragma mark - Setting up Gesture Recognizers

  bool setupGestureRecognizer(recognition::GestureRecognizer&& recognizer);

 private:
  core::EventLoop* _loop;
  geom::Size _size;
  std::mutex _lock;
  Layer::Ref _rootLayer;
  std::deque<InterfaceTransaction> _transactionStack;
  size_t _popCount;
  std::shared_ptr<core::EventLoopObserver> _autoFlushObserver;
  std::weak_ptr<InterfaceDelegate> _delegate;
  std::weak_ptr<coordinator::CompositorChannel> _compositorChannel;
  toolbox::StateMachine _state;

  void armAutoFlushTransactions(bool arm);
  void flushTransactions();
  void scheduleChannels();
  void unscheduleChannels();
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
