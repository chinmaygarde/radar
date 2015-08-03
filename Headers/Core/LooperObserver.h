// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef __RADARLOVER_CORE_LOOPEROBSERVER__
#define __RADARLOVER_CORE_LOOPEROBSERVER__

#include "Core/Base.h"

#include <functional>
#include <set>
#include <mutex>

namespace rl {
class LooperObserver {
 public:
  using Callback = std::function<void(void)>;

  enum Activity {
    /**
     *  Looper activities that take place just after the looper has processed
     *  all sources and is about to go to sleep.
     */
    BeforeSleep,

    /**
     *  Looper activities that take place after the looper has woken up but
     *  has still to process its sources.
     */
    AfterSleep,
  };

  /**
   *  Create a looper observer at the specified absolute priority
   */
  LooperObserver(uint64_t priority, Callback callback);

  /*
   *  Invoke the looper observer callback
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

  DISALLOW_COPY_AND_ASSIGN(LooperObserver);
};

struct LooperObserverComparer {
  bool operator()(std::shared_ptr<LooperObserver> a,
                  std::shared_ptr<LooperObserver> b) const;
};

class LooperObserverCollection {
 public:
  LooperObserverCollection();

  /**
   *  Adds an observer to the collection.
   *  Warning: Can deadlock if in the middle of an `invokeAll` call
   *
   *  @param observer the observer to add
   */
  void addObserver(std::shared_ptr<LooperObserver> observer);

  /**
   *  Removes an observer from the collection.
   *  Warning: Can deadlock if in the middle of an `invokeAll` call
   *
   *  @param observer the observer to remove
   */
  void removeObserver(std::shared_ptr<LooperObserver> observer);

  /**
   *  Invokes all member callbacks.
   */
  void invokeAll();

 private:
  using LooperObserversSet =
      std::set<std::shared_ptr<LooperObserver>, LooperObserverComparer>;
  std::mutex _lock;
  LooperObserversSet _observers;

  DISALLOW_COPY_AND_ASSIGN(LooperObserverCollection);
};
}

#endif /* defined(__RADARLOVER_CORE_LOOPEROBSERVER__) */
