// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef __RADARLOVE_INTERFACE_INTERFACE__
#define __RADARLOVE_INTERFACE_INTERFACE__

#include "Core/Base.h"
#include "Core/Looper.h"
#include "Core/Latch.h"
#include "Event/TouchEventChannel.h"
#include "Interface/InterfaceTransaction.h"
#include "Infrastructure/StateMachine.h"

#include <stack>
#include <mutex>

namespace rl {
class Interface {
 public:
  enum State {
    NotRunning,
    Inactive,
    Active,
    Background,
  };

  Interface();

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

#pragma mark - Transaction Management

  /**
   *  Get the transaction on top of the interface transaction stack
   *
   *  @return the top transaction
   */
  const InterfaceTransaction& transaction();

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

 private:
  Looper* _looper;
  std::mutex _lock;
  std::stack<InterfaceTransaction> _transactionStack;
  std::shared_ptr<LooperObserver> _autoFlushObserver;
  TouchEventChannel _touchEventChannel;
  StateMachine _state;

  void armAutoFlushTransactions(bool arm);
  void flushTransactions();
  void setupEventChannels();
  void processTouchEvents();

  void didFinishLaunching();
  void didEnterBackground();
  void didTerminate();
  void didBecomeActive();
  void didBecomeInactive();

  DISALLOW_COPY_AND_ASSIGN(Interface);
};
}

#endif /* defined(__RADARLOVE_INTERFACE_INTERFACE__) */
