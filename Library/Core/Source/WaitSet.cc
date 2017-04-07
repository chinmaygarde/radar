// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <Core/EventLoopSource.h>
#include <Core/Utilities.h>
#include <Core/WaitSet.h>
#include <algorithm>
#include "Config.h"
#include "EPollWaitSet.h"
#include "InProcessWaitSet.h"
#include "KQueueWaitSet.h"

namespace rl {
namespace core {

using PlatformWaitSetProvider =
#if RL_WAITSET == RL_WAITSET_KQUEUE
    KQueueWaitSet
#elif RL_WAITSET == RL_WAITSET_EPOLL
    EPollWaitSet
#elif RL_WAITSET == RL_WAITSET_INPROCESS
    InProcessWaitSet
#else
#error Unknown WaitSet Implementation
#endif
    ;

WaitSet::WaitSet() : _provider(std::make_unique<PlatformWaitSetProvider>()) {}

WaitSet::~WaitSet() {
  for (auto const& source : _sources) {
    _provider->updateSource(*this, *source, false);
  }
}

bool WaitSet::addSource(std::shared_ptr<EventLoopSource> source) {
  if (source == nullptr) {
    return false;
  }

  core::MutexLocker lock(_sourcesMutex);
  if (_sources.emplace(source).second) {
    _provider->updateSource(*this, *source, true);
    return true;
  }

  return false;
}

bool WaitSet::removeSource(std::shared_ptr<EventLoopSource> source) {
  if (source == nullptr) {
    return false;
  }

  core::MutexLocker lock(_sourcesMutex);
  if (_sources.erase(source) != 0) {
    _provider->updateSource(*this, *source, false);
    return true;
  }

  return false;
}

WaitSet::Result WaitSet::wait(ClockDurationNano timeout) {
  auto result = _provider->wait(timeout);

  /*
   *  In case of terminations initiated from the remote end, we need to cleanup
   *  our collection of sources.
   */
  if (result.first == WaitSet::WakeReason::Error && result.second != nullptr) {
    EventLoopSourceRef found;

    core::MutexLocker lock(_sourcesMutex);

    for (const auto& source : _sources) {
      if (source.get() == result.second) {
        found = source;
        break;
      }
    }

    if (found != nullptr) {
      _sources.erase(found);
    }
  }

  return result;
}

WaitSet::Handle WaitSet::handle() const {
  return _provider->handle();
}

WaitSetProvider& WaitSet::provider() const {
  return *_provider;
}

}  // namespace core
}  // namespace rl
