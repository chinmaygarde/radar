// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef __RADARLOVE_INTERFACE_INTERFACE__
#define __RADARLOVE_INTERFACE_INTERFACE__

#include <Core/Core.h>
#include <Event/TouchEventChannel.h>
#include <Interface/InterfaceTransaction.h>
#include <Interface/InterfaceDelegate.h>
#include <Infrastructure/StateMachine.h>
#include <Interface/Layer.h>
#include <Compositor/EntityLease.h>

#include <stack>
#include <mutex>

namespace rl {
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

  Interface(std::weak_ptr<InterfaceDelegate> delegate,
            EntityLease& interfaceLease);

#pragma mark - Bootstrapping the interface

  /**
   *  Setup the interface context and count down on the latch when ready
   *
   *  @param readyLatch the latch to count down on readiness
   */
  void run(Latch& onReady);

  /**
   *  @return if the interface context running
   */
  bool isRunning() const;

  /**
   *  Gracefully shutdown the interface
   */
  void shutdown(Latch& onShutdown);

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
  const Size& size() const;

  /**
   *  Update the size of the interface
   *
   *  @param size the new interface size
   */
  void setSize(const Size& size);

#pragma mark - Transaction Management

  /**
   *  Get the transaction on top of the interface transaction stack
   *
   *  @return the top transaction
   */
  InterfaceTransaction& transaction();

  /**
   *  Push a new transaction (with the given arguments) onto the transaction
   *  stack
   *
   *  @param args the arguments to the interface transaction
   */
  template <typename... T>
  void pushTransaction(T&&... args);

  /**
   *  Pop the last transaction off the transaction stack
   */
  void popTransaction();

#pragma mark - Accessing Event Channels

  /**
   *  Get the channel used for sending touch events to the interface
   *
   *  @return the touch event channel
   */
  TouchEventChannel& touchEventChannel();

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

 private:
  EventLoop* _loop;
  Size _size;
  std::mutex _lock;
  Layer::Ref _rootLayer;
  std::stack<InterfaceTransaction> _transactionStack;
  std::shared_ptr<EventLoopObserver> _autoFlushObserver;
  TouchEventChannel _touchEventChannel;
  std::weak_ptr<InterfaceDelegate> _delegate;
  EntityLease& _lease;
  StateMachine _state;

  void armAutoFlushTransactions(bool arm);
  void flushTransactions();
  void setupEventChannels();
  void cleanupEventChannels();
  void processTouchEvents();
  void performTerminationCleanup();
  void finalizeLeaseWrite();

  void didFinishLaunching();
  void didBecomeActive();
  void didEnterBackground();
  void didTerminate();
  void didBecomeInactive();
  void didUpdateSize();

  DISALLOW_COPY_AND_ASSIGN(Interface);
};
}

#endif /* defined(__RADARLOVE_INTERFACE_INTERFACE__) */
