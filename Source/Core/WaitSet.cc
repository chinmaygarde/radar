// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <Core/WaitSet.h>
#include <Core/Utilities.h>
#include <Core/LooperSource.h>

namespace rl {

WaitSet::WaitSet() : _handle(platformHandleCreate()) {
}

bool WaitSet::addSource(std::shared_ptr<LooperSource> source) {
  if (_sources.find(source) != _sources.end()) {
    return false;
  }

  _sources.insert(source);
  source->updateInWaitSetHandle(_handle, true);

  return true;
}

bool WaitSet::removeSource(std::shared_ptr<LooperSource> source) {
  if (_sources.find(source) == _sources.end()) {
    return false;
  }

  _sources.erase(source);
  source->updateInWaitSetHandle(_handle, false);

  return true;
}

LooperSource* WaitSet::wait() {
  return platformHandleWait(_handle);
}

WaitSet::~WaitSet() {
  for (auto const& source : _sources) {
    source->updateInWaitSetHandle(_handle, false);
  }

  platformHandleDestory(_handle);
}

}  // namespace rl
