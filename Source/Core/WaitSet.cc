// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <Core/WaitSet.h>
#include <Core/Utilities.h>
#include <Core/EventLoopSource.h>

#include <algorithm>

namespace rl {

WaitSet::WaitSet() : _handle(platformHandleCreate()) {
}

bool WaitSet::addSource(std::shared_ptr<EventLoopSource> source) {
  if (std::find(_sources.begin(), _sources.end(), source) != _sources.end()) {
    return false;
  }

  _sources.push_back(source);
  source->updateInWaitSetHandle(_handle, true);

  return true;
}

bool WaitSet::removeSource(std::shared_ptr<EventLoopSource> source) {
  auto found = std::find(_sources.begin(), _sources.end(), source);
  if (found == _sources.end()) {
    return false;
  }

  _sources.erase(found);
  source->updateInWaitSetHandle(_handle, false);

  return true;
}

EventLoopSource& WaitSet::wait() {
  return platformHandleWait(_handle);
}

WaitSet::~WaitSet() {
  for (auto const& source : _sources) {
    source->updateInWaitSetHandle(_handle, false);
  }

  platformHandleDestory(_handle);
}

}  // namespace rl
