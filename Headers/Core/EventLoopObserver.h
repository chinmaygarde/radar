// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef __RADARLOVER_CORE_EVENT_LOOP_OBSERVER__
#define __RADARLOVER_CORE_EVENT_LOOP_OBSERVER__

#include <Core/Macros.h>

#include <functional>
#include <set>
#include <mutex>

namespace rl {
class EventLoopObserver {
 public:
  using Callback = std::function<void(void)>;

  enum Activity {
    /**
     *  Event loop activities that take place just after the loop has processed
     *  all sources and is about to go to sleep.
     */
    BeforeSleep,

    /**
     *  Event loop activities that take place after the loop has woken up but
     *  has still to process its sources.
     */
    AfterSleep,
  };

  /**
   *  Create an event loop observer at the specified absolute priority
   */
  explicit EventLoopObserver(uint64_t priority, Callback callback);

  /*
   *  Invoke the event loop observer callback
   */
  void invoke() const;

  /**
   *  The absolute priority of the observer
   *
   *  @return the priority
   */
  uint64_t priority() const;

 private:
  const Callback _callback;
  const uint64_t _priority;

  DISALLOW_COPY_AND_ASSIGN(EventLoopObserver);
};

struct EventLoopObserverComparer {
  bool operator()(std::shared_ptr<EventLoopObserver> a,
                  std::shared_ptr<EventLoopObserver> b) const;
};

class EventLoopObserverCollection {
 public:
  explicit EventLoopObserverCollection();

  /**
   *  Adds an observer to the collection.
   *  Warning: Can deadlock if in the middle of an `invokeAll` call
   *
   *  @param observer the observer to add
   */
  void addObserver(std::shared_ptr<EventLoopObserver> observer);

  /**
   *  Removes an observer from the collection.
   *  Warning: Can deadlock if in the middle of an `invokeAll` call
   *
   *  @param observer the observer to remove
   */
  void removeObserver(std::shared_ptr<EventLoopObserver> observer);

  /**
   *  Invokes all member callbacks.
   */
  void invokeAll();

 private:
  using EventLoopObserversSet =
      std::set<std::shared_ptr<EventLoopObserver>, EventLoopObserverComparer>;
  std::mutex _lock;
  EventLoopObserversSet _observers;

  DISALLOW_COPY_AND_ASSIGN(EventLoopObserverCollection);
};
}

#endif /* defined(__RADARLOVER_CORE_EVENT_LOOP_OBSERVER__) */
