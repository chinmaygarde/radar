// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <Core/EventLoop.h>
#include <Core/ThreadLocal.h>
#include <Core/Utilities.h>

#include <mutex>

namespace rl {
namespace core {

RL_THREAD_LOCAL ThreadLocal CurrentEventLoop([](uintptr_t value) {
  delete reinterpret_cast<EventLoop*>(value);
});

EventLoop* EventLoop::Current() {
  auto loop = reinterpret_cast<EventLoop*>(CurrentEventLoop.get());
  if (loop != nullptr) {
    return loop;
  }

  loop = new EventLoop();
  CurrentEventLoop.set(reinterpret_cast<uintptr_t>(loop));
  return loop;
}

EventLoop::EventLoop()
    : _waitSet(),
      _trivialSource(nullptr),
      _shouldTerminate(false),
      _pendingDispatches(make_unique<PendingBlocks>()),
      _beforeSleepObservers(EventLoopObserver::Activity::BeforeSleep),
      _afterSleepObservers(EventLoopObserver::Activity::AfterSleep) {}

EventLoop::~EventLoop() {}

bool EventLoop::addSource(std::shared_ptr<EventLoopSource> source) {
  return _waitSet.addSource(source);
}

bool EventLoop::removeSource(std::shared_ptr<EventLoopSource> source) {
  return _waitSet.removeSource(source);
}

void EventLoop::loop(std::function<void(void)> onReady) {
  if (_trivialSource.get() == nullptr) {
    /*
     *  A trivial source needs to be added to keep the loop idle
     *  without any other sources present.
     */
    _trivialSource = EventLoopSource::Trivial();
    addSource(_trivialSource);
  }

  if (onReady) {
    onReady();
  }

  beforeSleep();

  while (!_shouldTerminate) {
    auto timeout = ClockDurationNano::max();

    while (true) {
      /*
       * =======================================================================
       * Step 0: Wait
       * =======================================================================
       */

      /*
       *  Sleep indefinitely the first time around
       */
      auto* source = _waitSet.wait(timeout);

      /*
       * =======================================================================
       * Step 1: Notify observers on wake
       * =======================================================================
       */

      if (timeout == ClockDurationNano::max()) {
        /*
         *  The first wake after an indefinite sleep means that the eventloop
         *  is about to service all pending reads. Notify our loop observers of
         *  the wake
         */
        afterSleep();
      }

      /*
       * =======================================================================
       * Step 2: Attempt reading on source
       * =======================================================================
       */
      if (source != nullptr) {
        /*
         *  Attempt read on the signalled source. The source may be `nullptr` if
         *  this is not the first read (with a zero timeout)
         */
        source->attemptRead();
      }

      /*
       * =======================================================================
       * Step 3: Retry or sleep
       * =======================================================================
       */
      if (timeout == ClockDurationNano::max()) {
        /*
         *  After the first read, all subsequent reads are with a zero timeout.
         */
        timeout = ClockDurationNano(0);
      } else {
        /*
         *  We are here because of a zero timeout read. If the source is not
         *  nullptr, try again to see if any pending reads are available.
         */
        if (source == nullptr || _shouldTerminate) {
          /*
           *  We got a nullptr source on a zero timeout read on the waitset.
           *  There is absolutely nothing to be read on the waitset and we may
           *  go to sleep now. Notify our observers accordingly.
           */
          beforeSleep();
          break;
        }
      }
    }
  }

  afterSleep();

  _shouldTerminate = false;
}

void EventLoop::terminate() {
  if (_shouldTerminate) {
    return;
  }

  _shouldTerminate = true;

  /*
   *  In case the terminate call is made before the call to run, the trivial
   *  source will be null.
   */
  if (_trivialSource) {
    _trivialSource->writer()(_trivialSource->writeHandle());
  }
}

void EventLoop::beforeSleep() {
  flushPendingDispatches();
  _beforeSleepObservers.invokeAll();
}

void EventLoop::afterSleep() {
  flushPendingDispatches();
  _afterSleepObservers.invokeAll();
}

void EventLoop::flushPendingDispatches() {
  std::unique_ptr<std::list<Block>> pending;

  {
    /*
     *  Hold the lock for as short of a time as possible. Release the lock while
     *  flushing dispatches
     */
    std::lock_guard<std::mutex> lock(_lock);
    if (_pendingDispatches->size() != 0) {
      pending.swap(_pendingDispatches);
      _pendingDispatches = make_unique<PendingBlocks>();
    }
  }

  if (pending != nullptr) {
    /*
     *  No lock is held here. So callouts can queue more dispatches
     */
    for (const auto& block : *pending) {
      block();
    }
  }
}

void EventLoop::dispatchAsync(std::function<void()> block) {
  RL_ASSERT_MSG(_trivialSource, "A trivial source must be present");

  std::lock_guard<std::mutex> lock(_lock);
  _pendingDispatches->push_back(block);
  _trivialSource->writer()(_trivialSource->writeHandle());
}

bool EventLoop::addObserver(std::shared_ptr<EventLoopObserver> observer,
                            EventLoopObserver::Activity activity) {
  switch (activity) {
    case EventLoopObserver::Activity::BeforeSleep:
      return _beforeSleepObservers.addObserver(observer);
    case EventLoopObserver::Activity::AfterSleep:
      return _afterSleepObservers.addObserver(observer);
  }

  return false;
}

bool EventLoop::removeObserver(std::shared_ptr<EventLoopObserver> observer,
                               EventLoopObserver::Activity activity) {
  switch (activity) {
    case EventLoopObserver::Activity::BeforeSleep:
      return _beforeSleepObservers.removeObserver(observer);
    case EventLoopObserver::Activity::AfterSleep:
      return _afterSleepObservers.removeObserver(observer);
  }
}

}  // namespace core
}  // namespace rl
