// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <Core/Config.h>
#include <Core/EventLoopSource.h>
#include <Core/Utilities.h>
#include <Core/WaitSet.h>

#include <algorithm>

#include "EPollWaitSet.h"
#include "InProcessWaitSet.h"
#include "KQueueWaitSet.h"

namespace rl {
namespace core {

WaitSet::WaitSet() {
#if RL_WAITSET == RL_WAITSET_KQUEUE
  _provider = core::make_unique<KQueueWaitSet>();
#elif RL_WAITSET == RL_WAITSET_EPOLL
  _provider = core::make_unique<EPollWaitSet>();
#elif RL_WAITSET == RL_WAITSET_INPROCESS
  _provider = core::make_unique<InProcessWaitSet>();
#else
#error Unknown WaitSet Implementation
#endif
}

bool WaitSet::addSource(std::shared_ptr<EventLoopSource> source) {
  if (std::find(_sources.begin(), _sources.end(), source) != _sources.end()) {
    return false;
  }

  _sources.push_back(source);
  _provider->updateSource(*this, *source, true);

  return true;
}

bool WaitSet::removeSource(std::shared_ptr<EventLoopSource> source) {
  auto found = std::find(_sources.begin(), _sources.end(), source);
  if (found == _sources.end()) {
    return false;
  }

  _sources.erase(found);
  _provider->updateSource(*this, *source, false);

  return true;
}

EventLoopSource* WaitSet::wait(ClockDurationNano timeout) {
  return _provider->wait(timeout);
}

WaitSet::Handle WaitSet::handle() const {
  return _provider->handle();
}

void WaitSet::signalReadReadinessFromUserspace(
    EventLoopSource::Handle writeHandle) {
  return _provider->signalReadReadinessFromUserspace(writeHandle);
}

WaitSet::~WaitSet() {
  for (auto const& source : _sources) {
    _provider->updateSource(*this, *source, false);
  }
}

}  // namespace core
}  // namespace rl
