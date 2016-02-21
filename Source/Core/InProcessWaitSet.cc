// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <Core/Config.h>

#if RL_WAITSET == RL_WAITSET_INPROCESS

#include <Core/InProcessWaitSet.h>

#include <algorithm>

namespace rl {
namespace core {

InProcessWaitSet::InProcessWaitSet() {}

InProcessWaitSet::~InProcessWaitSet() {
  RL_ASSERT(_watchedSources.size() == 0);
}

static constexpr EventLoopSource::Handle TimerHandle() {
  return std::numeric_limits<EventLoopSource::Handle>::max();
}

static constexpr bool IsTimer(const EventLoopSource::Handles& handles) {
  return handles.first == TimerHandle();
}

static std::chrono::nanoseconds TimerIntervalRelative(
    const EventLoopSource::Handles& handles) {
  return std::chrono::nanoseconds(handles.second);
}

EventLoopSource::Handles InProcessWaitSet::TimerHandles(
    const std::chrono::nanoseconds& interval) {
  return EventLoopSource::Handles(TimerHandle(), interval.count());
}

WaitSet::Handle InProcessWaitSet::handle() const {
  return WaitSet::Handle(this);
}

void InProcessWaitSet::updateSource(WaitSet& waitset,
                                    EventLoopSource& source,
                                    bool addedOrRemoved) {
  addOrRemoveSource(source, addedOrRemoved);

  /*
   *  Custom wait set update handlers may trigger a "userspace" signal. We don't
   *  want to be holding onto the lock in case this happens. This is why the
   *  additional scope is introduced.
   */
  WaitSetProvider::updateSource(waitset, source, addedOrRemoved);

  /*
   *  Notify waiting members or source availability
   */
  _readySourcesCV.notify_all();
}

void InProcessWaitSet::addOrRemoveSource(EventLoopSource& source,
                                         bool addedOrRemoved) {
  std::lock_guard<std::mutex> lock(_readySourcesMutex);

  if (IsTimer(source.handles())) {
    if (addedOrRemoved) {
      setupTimer(source, TimerClock::now());
    } else {
      teardownTimer(source);
    }
  } else {
    if (addedOrRemoved) {
      setupSource(source);
    } else {
      teardownSource(source);
    }
  }
}

void InProcessWaitSet::setupTimer(EventLoopSource& source,
                                  TimerClockPoint now) {
  RL_ASSERT(IsTimer(source.handles()));

  auto absTimeout = now + TimerIntervalRelative(source.handles());

  _timers.emplace_back(source, absTimeout);
  std::push_heap(_timers.begin(), _timers.end(), ActiveTimerCompare());
}

void InProcessWaitSet::teardownTimer(EventLoopSource& source) {
  RL_ASSERT(IsTimer(source.handles()));

  auto found = std::find_if(
      _timers.begin(), _timers.end(),
      [&source](const ActiveTimer& timer) { return timer.source == &source; });

  RL_ASSERT(found != _timers.end());

  _timers.erase(found);
  std::make_heap(_timers.begin(), _timers.end(), ActiveTimerCompare());
}

void InProcessWaitSet::setupSource(EventLoopSource& source) {
  _watchedSources[source.writeHandle()] = &source;
}

void InProcessWaitSet::teardownSource(EventLoopSource& source) {
  _watchedSources.erase(source.writeHandle());
}

bool InProcessWaitSet::isTimerExpired(TimerClockPoint now) const {
  if (_timers.size() == 0) {
    return false;
  }

  return now > _timers.front().absoluteTimeout;
}

bool InProcessWaitSet::isAwakable() const {
  bool hasReadySources = _readySources.size() > 0;

  return hasReadySources ||
         isTimerExpired(TimerClock::now()) /* syscall so last */;
}

InProcessWaitSet::TimerClockPoint InProcessWaitSet::nextTimerTimeout(
    TimerClockPoint upperBound) const {
  auto next = _timers.size() == 0 ? TimerClockPoint::max()
                                  : _timers.front().absoluteTimeout;

  if (next > upperBound) {
    return upperBound;
  }

  return next;
}

EventLoopSource* InProcessWaitSet::wait(ClockDurationNano timeout) {
  /*
   *  Try to see if there are any sources signalled without waiting on the
   *  condition variable
   */
  {
    std::lock_guard<std::mutex> lock(_readySourcesMutex);
    if (auto source = timerOrSourceOnWakeNoLock(TimerClock::now())) {
      return source;
    }
  }

  /*
   *  There were no pre-signalled sources and we are forced to wait on the
   *  condition variable
   */
  std::unique_lock<std::mutex> lock(_readySourcesMutex);

  auto upperBound = TimerClock::now() +
                    std::chrono::duration_cast<TimerClockDuration>(timeout);

  auto satisfied = _readySourcesCV.wait_until(
      lock, nextTimerTimeout(upperBound),
      std::bind(&InProcessWaitSet::isAwakable, this));

  return satisfied ? timerOrSourceOnWakeNoLock(TimerClock::now()) : nullptr;
}

EventLoopSource* InProcessWaitSet::timerOrSourceOnWakeNoLock(
    TimerClockPoint now) {
  if (auto source = sourceOnWakeNoLock()) {
    return source;
  }
  return timerOnWakeNoLock(now);
}

EventLoopSource* InProcessWaitSet::timerOnWakeNoLock(TimerClockPoint now) {
  if (!isTimerExpired(now)) {
    return nullptr;
  }

  /*
   *  Grab a hold of the next signalled timer
   */
  auto source = _timers.front().source;
  RL_ASSERT(source);

  /*
   *  Erase the expired timer from the heap. We don't use teardownTimer() since
   *  that assumes that any timer (not just the top of the heap) may be removed.
   */
  std::pop_heap(_timers.begin(), _timers.end(), ActiveTimerCompare());
  _timers.pop_back();

  /*
   *  re-arm the timer since all timers are repeating
   */
  setupTimer(*source, now);

  return source;
}

EventLoopSource* InProcessWaitSet::sourceOnWakeNoLock() {
  RL_ASSERT(!_readySourcesMutex.try_lock());

  auto found = _readySources.begin();

  if (found == _readySources.end()) {
    return nullptr;
  }

  auto source = *found;
  _readySources.erase(found);

  return source;
}

void InProcessWaitSet::signalReadReadinessFromUserspace(
    EventLoopSource::Handle writeHandle) {
  {
    std::lock_guard<std::mutex> lock(_readySourcesMutex);
    auto result = _watchedSources[writeHandle];
    RL_ASSERT(result != nullptr);
    _readySources.insert(result);
  }

  _readySourcesCV.notify_all();
}

}  // namespace core
}  // namespace rl

#endif  // RL_WAITSET == RL_WAITSET_INPROCESS
