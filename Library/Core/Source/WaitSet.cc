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
  if (source == nullptr) {
    return false;
  }

  std::lock_guard<std::mutex> lock(_sourcesMutex);

  if (std::find(_sources.begin(), _sources.end(), source) != _sources.end()) {
    return false;
  }

  _sources.push_back(source);
  _provider->updateSource(*this, *source, true);

  return true;
}

bool WaitSet::removeSource(std::shared_ptr<EventLoopSource> source) {
  if (source == nullptr) {
    return false;
  }

  std::lock_guard<std::mutex> lock(_sourcesMutex);

  auto found = std::find(_sources.begin(), _sources.end(), source);
  if (found == _sources.end()) {
    return false;
  }

  _sources.erase(found);
  _provider->updateSource(*this, *source, false);

  return true;
}

WaitSet::Result WaitSet::wait(ClockDurationNano timeout) {
  auto result = _provider->wait(timeout);

  /*
   *  In case of terminations initiated from the remote end, we need to cleanup
   *  our collection of sources.
   */
  if (result.first == WaitSet::WakeReason::Error && result.second != nullptr) {
    std::lock_guard<std::mutex> lock(_sourcesMutex);

    auto errorItem = result.second;

    auto found =
        std::find_if(_sources.begin(), _sources.end(),
                     [errorItem](const std::shared_ptr<EventLoopSource>& item) {
                       return item.get() == errorItem;
                     });

    if (found != _sources.end()) {
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

WaitSet::~WaitSet() {
  std::lock_guard<std::mutex> lock(_sourcesMutex);

  for (auto const& source : _sources) {
    _provider->updateSource(*this, *source, false);
  }
}

}  // namespace core
}  // namespace rl
