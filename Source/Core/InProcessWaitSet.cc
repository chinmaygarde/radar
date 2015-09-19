// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "InProcessWaitSet.h"

#include <algorithm>

namespace rl {

InProcessWaitSet::InProcessWaitSet() : _idleWake(false) {
}

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
                                    bool added) {
  WaitSetProvider::updateSource(waitset, source, added);

  std::lock_guard<std::mutex> lock(_lock);

  if (IsTimer(source.handles())) {
    if (added) {
      setupTimer(source);
    } else {
      teardownTimer(source);
    }
    /*
     *  in care timer re-arms are required, notify an idle wake
     */
    _idleWake = true;
    _conditionVariable.notify_all();
  } else {
    if (added) {
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

  return _idleWake || hasReadySources || isTimerExpired() /* syscall so last */;
}

InProcessWaitSet::TimerClockPoint InProcessWaitSet::nextTimeout() const {
  return _timers.size() == 0 ? TimerClockPoint::max()
                             : _timers.front().absoluteTimeout;
}

EventLoopSource& InProcessWaitSet::wait() {
  EventLoopSource* source = nullptr;

  while (true) {
    std::unique_lock<std::mutex> lock(_lock);
    _conditionVariable.wait_until(lock, nextTimeout(),
                                  [&] { return isAwakable(); });

    auto now = TimerClock::now();
    source = isTimerExpired(now) ? &timerOnWake(now) : sourceOnWake();

    lock.unlock();

    if (source == nullptr && _idleWake) {
      /*
       *  We could not find a signalled source or timer, but this was an idle
       *  wake anyway. Try again.
       */
      _idleWake = false;
      continue;
    }

    _idleWake = false;
    break;
  }

  RL_ASSERT(source != nullptr);
  return *source;
}

EventLoopSource& InProcessWaitSet::timerOnWake(TimerClockPoint now) {
  RL_ASSERT(isTimerExpired(now));

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

  return *source;
}

EventLoopSource* InProcessWaitSet::sourceOnWake() {
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
  std::lock_guard<std::mutex> lock(_lock);

  auto result = _watchedSources[writeHandle];
  RL_ASSERT(result != nullptr);
  _readySources.insert(result);

  _conditionVariable.notify_all();
}

}  // namespace rl
