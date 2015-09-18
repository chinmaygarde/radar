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
  return -1;
}

void InProcessWaitSet::updateSource(EventLoopSource& source, bool added) {
  WaitSetProvider::updateSource(source, added);

  if (added) {
    _watchedSources.insert(&source);
  } else {
    _watchedSources.erase(&source);
  }
}

// EventLoopSource& InProcessWaitSet::wait() {
//  return *reinterpret_cast<EventLoopSource*>(nullptr);
//}

}  // namespace rl
