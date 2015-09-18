// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <Core/Config.h>
#include <Core/WaitSet.h>
#include <Core/Utilities.h>
#include <Core/EventLoopSource.h>

#include <algorithm>

#include "KQueueWaitSet.h"
#include "EPollWaitSet.h"
#include "InProcessWaitSet.h"

namespace rl {

WaitSet::WaitSet() {
#if RL_WAITSET == RL_WAITSET_KQUEUE
  _provider = rl::make_unique<KQueueWaitSet>();
#elif RL_WAITSET == RL_WAITSET_EPOLL
  _provider = rl::make_unique<EPollWaitSet>();
#elif RL_WAITSET == RL_WAITSET_INPROCESS
  _provider = rl::make_unique<InProcessWaitSet>();
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

EventLoopSource& WaitSet::wait() {
  return _provider->wait();
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

}  // namespace rl
