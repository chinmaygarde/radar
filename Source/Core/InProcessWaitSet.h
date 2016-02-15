// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef RADARLOVE_CORE_INPROCESSWAITSET_H_
#define RADARLOVE_CORE_INPROCESSWAITSET_H_

#include <Core/Config.h>

#if RL_WAITSET == RL_WAITSET_INPROCESS

#include <Core/Macros.h>
#include <Core/WaitSetProvider.h>

#include <condition_variable>
#include <queue>
#include <unordered_map>
#include <unordered_set>

namespace rl {
namespace core {

class InProcessWaitSet : public WaitSetProvider {
 public:
  InProcessWaitSet();
  ~InProcessWaitSet();

  static EventLoopSource::Handles TimerHandles(
      const std::chrono::nanoseconds& interval);

  EventLoopSource* wait(ClockDurationNano timeout) override;
  WaitSet::Handle handle() const override;
  void signalReadReadinessFromUserspace(
      EventLoopSource::Handle writeHandle) override;
  void updateSource(WaitSet& waitset,
                    EventLoopSource& source,
                    bool addedOrRemoved) override;

 private:
  using TimerClock = std::chrono::high_resolution_clock;
  using TimerClockDuration = ClockDurationSeconds;
  using TimerClockPoint =
      std::chrono::time_point<TimerClock, TimerClockDuration>;

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

  std::condition_variable _conditionVariable;
  WriteHandleSourcesMap _watchedSources;
  ActiveTimersHeap _timers;
  std::mutex _readySourcesMutex;
  std::unordered_set<EventLoopSource*> _readySources;

  TimerClockPoint nextTimerTimeout(TimerClockPoint upperBound) const;
  bool isAwakable() const;
  bool isTimerExpired(TimerClockPoint now) const;

  void setupTimer(EventLoopSource& source, TimerClockPoint now);
  void teardownTimer(EventLoopSource& source);

  void setupSource(EventLoopSource& source);
  void teardownSource(EventLoopSource& source);

  EventLoopSource* timerOrSourceOnWakeNoLock(TimerClockPoint now);
  EventLoopSource* timerOnWakeNoLock(TimerClockPoint now);
  EventLoopSource* sourceOnWakeNoLock();

  RL_DISALLOW_COPY_AND_ASSIGN(InProcessWaitSet);
};

}  // namespace core
}  // namespace rl

#endif  // RL_WAITSET == RL_WAITSET_INPROCESS

#endif  // RADARLOVE_CORE_INPROCESSWAITSET_H_
