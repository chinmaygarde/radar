// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef RADARLOVER_CORE_EVENT_LOOP_OBSERVER_
#define RADARLOVER_CORE_EVENT_LOOP_OBSERVER_

#include <Core/Macros.h>
#include <Core/Mutex.h>
#include <functional>
#include <memory>
#include <set>

namespace rl {
namespace core {

class EventLoopObserver {
 public:
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

  using Callback = std::function<void(Activity)>;

  /**
   *  Create an event loop observer at the specified absolute priority
   */
  EventLoopObserver(Callback callback, int64_t priority = 0);

  /*
   *  Invoke the event loop observer callback
   */
  void invoke(Activity activity) const;

  /**
   *  The absolute priority of the observer
   *
   *  @return the priority
   */
  int64_t priority() const;

 private:
  const Callback _callback;
  const int64_t _priority;

  RL_DISALLOW_COPY_AND_ASSIGN(EventLoopObserver);
};

struct EventLoopObserverComparer {
  bool operator()(std::shared_ptr<EventLoopObserver> a,
                  std::shared_ptr<EventLoopObserver> b) const;
};

class EventLoopObserverCollection {
 public:
  EventLoopObserverCollection(EventLoopObserver::Activity activity);

  /**
   *  Adds an observer to the collection.
   *  Warning: Can deadlock if in the middle of an `invokeAll` call
   *
   *  @param observer the observer to add
   */
  bool addObserver(std::shared_ptr<EventLoopObserver> observer);

  /**
   *  Removes an observer from the collection.
   *  Warning: Can deadlock if in the middle of an `invokeAll` call
   *
   *  @param observer the observer to remove
   */
  bool removeObserver(std::shared_ptr<EventLoopObserver> observer);

  /**
   *  Invokes all member callbacks.
   */
  void invokeAll();

 private:
  using EventLoopObserversSet =
      std::set<std::shared_ptr<EventLoopObserver>, EventLoopObserverComparer>;
  EventLoopObserver::Activity _activity;
  Mutex _observersMutex;
  EventLoopObserversSet _observers RL_GUARDED_BY(_observersMutex);

  RL_DISALLOW_COPY_AND_ASSIGN(EventLoopObserverCollection);
};

}  // namespace core
}  // namespace rl

#endif  // RADARLOVER_CORE_EVENT_LOOP_OBSERVER_
