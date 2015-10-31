// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "InProcessTrivialSource.h"

#include <Core/WaitSet.h>

namespace rl {
namespace core {

InProcessTrivialSource::InProcessTrivialSource() : EventLoopSource() {
  setHandlesProvider([&] {
    auto handle = reinterpret_cast<EventLoopSource::Handle>(this);
    return EventLoopSource::Handles(handle, handle);
  });

  setWriter([&](Handle handle) {
    for (const auto& waitset : _activeWaitSets) {
      waitset->signalReadReadinessFromUserspace(
          reinterpret_cast<EventLoopSource::Handle>(this));
    }
  });

  setCustomWaitSetUpdateHandler([&](EventLoopSource& source, WaitSet& waitset,
                                    Handle readHandle, bool adding) {
    if (adding) {
      _activeWaitSets.insert(&waitset);
    } else {
      _activeWaitSets.erase(&waitset);
    }
  });
}

InProcessTrivialSource::~InProcessTrivialSource() {
  RL_ASSERT(_activeWaitSets.size() == 0);
}

}  // namespace core
}  // namespace rl
