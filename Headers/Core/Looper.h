// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef __RADARLOVE__LOOPER__
#define __RADARLOVE__LOOPER__

#include "Core/Base.h"
#include "Core/LooperSource.h"
#include "Core/WaitSet.h"
#include "Core/Lock.h"
#include "Core/LooperObserver.h"

#include <functional>
#include <list>

namespace rl {

class Looper {
 public:
  using Block = std::function<void(void)>;
  using PendingBlocks = std::list<Block>;

  /**
   *  Loops the looper
   *
   *  @param onReady the function to call when the looper has finished setting
   *         and is about to wait for the first time
   */
  void loop(std::function<void(void)> onReady = nullptr);

  /**
   *  Terminate the looper
   */
  void terminate();

  /**
   *  Fetch the looper associated with the current thread
   *
   *  @return the looper associated with the thread
   */
  static Looper* Current();

  /**
   *  Add a custom source to the looper
   *
   *  @param source the source to add
   *
   *  @return if the source was added successfully
   */
  bool addSource(std::shared_ptr<LooperSource> source);

  /**
   *  Removes a previously added custom source from the looper
   *
   *  @param source the source to remove
   *
   *  @return if the source was succesfully removed
   */
  bool removeSource(std::shared_ptr<LooperSource> source);

  /**
   *  Perform a block on this looper at the next opportune moment
   *
   *  @param block the block to perform on the looper
   */
  void dispatchAsync(Block block);

  /**
   *  Add an observer that will be invoked during the specifed looper activity
   *
   *  @param observer the observer to add
   *  @param activity the activity to observe
   */
  void addObserver(std::shared_ptr<LooperObserver> observer,
                   LooperObserver::Activity activity);

  /**
   *  Remove an observer that was previously added for the specified looper
   *  activity
   *
   *  @param observer the observer to remove
   *  @param activity the activity that was being observed
   */
  void removeObserver(std::shared_ptr<LooperObserver> observer,
                      LooperObserver::Activity activity);

 private:
  Looper();
  ~Looper();
  void beforeSleep();
  void afterSleep();
  void flushPendingDispatches();

  WaitSet _waitSet;
  std::shared_ptr<LooperSource> _trivialSource;
  bool _shouldTerminate;
  Lock _lock;
  std::unique_ptr<PendingBlocks> _pendingDispatches;
  LooperObserverCollection _beforeSleepObservers;
  LooperObserverCollection _afterSleepObservers;

  DISALLOW_COPY_AND_ASSIGN(Looper);
};
}

#endif /* defined(__RADARLOVE__LOOPER__) */
