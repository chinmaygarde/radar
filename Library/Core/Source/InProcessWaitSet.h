// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef RADAR_CORE_INPROCESSWAITSET_H_
#define RADAR_CORE_INPROCESSWAITSET_H_

#include "Config.h"

#if RL_WAITSET == RL_WAITSET_INPROCESS

#include <Core/Macros.h>
#include <condition_variable>
#include <queue>
#include <unordered_map>
#include <unordered_set>
#include "WaitSetProvider.h"

namespace rl {
namespace core {

class InProcessWaitSet : public WaitSetProvider {
 public:
  InProcessWaitSet();

  ~InProcessWaitSet();

  static EventLoopSource::Handles TimerHandles(
      const std::chrono::nanoseconds& interval);

  WaitSet::Result wait(ClockDurationNano timeout) override;

  WaitSet::Handle handle() const override;

  void signalReadReadinessFromUserspace(
      const std::vector<EventLoopSource::Handle>& handles);

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

  WriteHandleSourcesMap _watchedSources;
  ActiveTimersHeap _timers;

  std::condition_variable _readySourcesCV;
  std::mutex _readySourcesMutex;
  std::unordered_set<EventLoopSource*> _readySources;

  void addOrRemoveSource(EventLoopSource& source, bool addedOrRemoved);

  bool markHandlesReady(const std::vector<EventLoopSource::Handle>& handles);

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

#endif  // RADAR_CORE_INPROCESSWAITSET_H_
