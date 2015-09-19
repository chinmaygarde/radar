// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef __RADARLOVE_CORE_INPROCESSWAITSET_H__
#define __RADARLOVE_CORE_INPROCESSWAITSET_H__

#include <Core/Macros.h>
#include <Core/WaitSetProvider.h>

#include <unordered_map>
#include <unordered_set>
#include <queue>
#include <condition_variable>

namespace rl {

class InProcessWaitSet : public WaitSetProvider {
 public:
  InProcessWaitSet();
  ~InProcessWaitSet();

  static EventLoopSource::Handles TimerHandles(
      const std::chrono::nanoseconds& interval);

  EventLoopSource& wait() override;
  WaitSet::Handle handle() const override;
  void signalReadReadinessFromUserspace(
      EventLoopSource::Handle writeHandle) override;
  void updateSource(WaitSet& waitset,
                    EventLoopSource& source,
                    bool addedOrRemoved) override;

 private:
  using TimerClock = std::chrono::high_resolution_clock;
  using TimerClockPoint = std::chrono::time_point<TimerClock>;

  struct ActiveTimer {
    EventLoopSource* source;
    TimerClockPoint absoluteTimeout;

    ActiveTimer(EventLoopSource& src, const TimerClockPoint& timeout)
        : source(&src), absoluteTimeout(timeout) {}
  };

  struct ActiveTimerCompare {
    bool operator()(const ActiveTimer& a, const ActiveTimer& b) {
      return a.absoluteTimeout > b.absoluteTimeout;
    }
  };

  using WriteHandleSourcesMap =
      std::unordered_map<EventLoopSource::Handle, EventLoopSource*>;
  using ActiveTimersHeap = std::vector<ActiveTimer>;

  std::mutex _lock;
  std::condition_variable _conditionVariable;
  bool _idleWake;
  WriteHandleSourcesMap _watchedSources;
  ActiveTimersHeap _timers;
  std::unordered_set<EventLoopSource*> _readySources;

  TimerClockPoint nextTimeout() const;
  bool isAwakable() const;
  bool isTimerExpired(TimerClockPoint now = TimerClock::now()) const;
  void setupTimer(EventLoopSource& source,
                  TimerClockPoint now = TimerClock::now());
  void teardownTimer(EventLoopSource& source);
  void setupSource(EventLoopSource& source);
  void teardownSource(EventLoopSource& source);
  EventLoopSource& timerOnWake(TimerClockPoint now = TimerClock::now());
  EventLoopSource* sourceOnWake();

  RL_DISALLOW_COPY_AND_ASSIGN(InProcessWaitSet);
};

}  // namespace rl

#endif  // __RADARLOVE_CORE_INPROCESSWAITSET_H__
