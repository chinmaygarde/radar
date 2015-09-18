// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "InProcessWaitSet.h"

namespace rl {

InProcessWaitSet::InProcessWaitSet() {
}

InProcessWaitSet::~InProcessWaitSet() {
  RL_ASSERT(_watchedSources.size() == 0);
}

WaitSet::Handle InProcessWaitSet::handle() const {
  return WaitSet::Handle(this);
}

void InProcessWaitSet::updateSource(WaitSet& waitset,
                                    EventLoopSource& source,
                                    bool added) {
  WaitSetProvider::updateSource(waitset, source, added);

  auto isTimer = source.writeHandle() == TimerHandle();
  if (added) {
    if (isTimer) {
      _watchedTimers.insert(&source);
    } else {
      _watchedSources[source.writeHandle()] = &source;
    }
  } else {
    if (isTimer) {
      _watchedTimers.erase(&source);
    } else {
      _watchedSources.erase(source.writeHandle());
    }
  }
}

EventLoopSource& InProcessWaitSet::wait() {
  std::unique_lock<std::mutex> lock(_lock);

  _conditionVariable.wait(lock, [&] { return _readySources.size() > 0; });

  auto found = _readySources.begin();
  RL_ASSERT(found != _readySources.end() &&
            "There must already be at least one ready source");

  auto source = *found;

  _readySources.erase(found);

  lock.unlock();

  return *source;
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
