// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef __RADARLOVE_CORE_EVENTLOOP__
#define __RADARLOVE_CORE_EVENTLOOP__

#include <Core/Macros.h>
#include <Core/EventLoopSource.h>
#include <Core/WaitSet.h>
#include <Core/EventLoopObserver.h>

#include <functional>
#include <list>
#include <mutex>

namespace rl {

class EventLoop {
 public:
  using Block = std::function<void(void)>;
  using PendingBlocks = std::list<Block>;

  /**
   *  Starts the event loop
   *
   *  @param onReady the function to call when the loop has finished setting
   *         and is about to wait for the first time
   */
  void loop(std::function<void(void)> onReady = nullptr);

  /**
   *  Terminate the event loop
   */
  void terminate();

  /**
   *  Fetch the event loop associated with the current thread
   *
   *  @return the event loop associated with the thread
   */
  static EventLoop* Current();

  /**
   *  Add a custom source to the event loop
   *
   *  @param source the source to add
   *
   *  @return if the source was added successfully to the event loop
   */
  bool addSource(std::shared_ptr<EventLoopSource> source);

  /**
   *  Removes a previously added custom source from the event loop
   *
   *  @param source the source to remove
   *
   *  @return if the source was succesfully removed
   */
  bool removeSource(std::shared_ptr<EventLoopSource> source);

  /**
   *  Perform a block on this event loop at the next opportune moment
   *
   *  @param block the block to perform on the loop
   */
  void dispatchAsync(Block block);

  /**
   *  Add an observer that will be invoked during the specifed event loop
   *  activity
   *
   *  @param observer the observer to add
   *  @param activity the activity to observe
   */
  void addObserver(std::shared_ptr<EventLoopObserver> observer,
                   EventLoopObserver::Activity activity);

  /**
   *  Remove an observer that was previously added for the specified event loop
   *  activity
   *
   *  @param observer the observer to remove
   *  @param activity the activity that was being observed
   */
  void removeObserver(std::shared_ptr<EventLoopObserver> observer,
                      EventLoopObserver::Activity activity);

 private:
  WaitSet _waitSet;
  std::shared_ptr<EventLoopSource> _trivialSource;
  bool _shouldTerminate;
  std::mutex _lock;
  std::unique_ptr<PendingBlocks> _pendingDispatches;
  EventLoopObserverCollection _beforeSleepObservers;
  EventLoopObserverCollection _afterSleepObservers;

  explicit EventLoop();
  ~EventLoop();
  void beforeSleep();
  void afterSleep();
  void flushPendingDispatches();

  DISALLOW_COPY_AND_ASSIGN(EventLoop);
};
}

#endif /* defined(__RADARLOVE_CORE_EVENTLOOP__) */