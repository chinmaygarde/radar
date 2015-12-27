// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <Core/Config.h>

#if RL_WAITSET == RL_WAITSET_INPROCESS

#include <Core/InProcessTrivialSource.h>

#include <Core/WaitSet.h>

namespace rl {
namespace core {

InProcessTrivialSource::InProcessTrivialSource()
    : EventLoopSource(), _signalled(false) {
  setHandlesProvider([&] {
    auto handle = reinterpret_cast<EventLoopSource::Handle>(this);
    return EventLoopSource::Handles(handle, handle);
  });

  setWriter([&](Handle handle) {
    std::lock_guard<std::mutex> lock(_lock);
    _signalled = true;
    for (const auto& waitset : _activeWaitSets) {
      waitset->signalReadReadinessFromUserspace(
          reinterpret_cast<EventLoopSource::Handle>(this));
    }
    return IOResult::Success;
  });

  setReader([&](Handle) {
    std::lock_guard<std::mutex> lock(_lock);
    _signalled = false;
    return IOResult::Success;
  });

  setCustomWaitSetUpdateHandler([&](EventLoopSource& source, WaitSet& waitset,
                                    Handle readHandle, bool adding) {
    std::lock_guard<std::mutex> lock(_lock);

    if (adding) {
      _activeWaitSets.insert(&waitset);
    } else {
      _activeWaitSets.erase(&waitset);
    }

    /*
     *  The source could have been signalled before being added to the waitset.
     *  In this case, the writer failed to notify any waitsets. Now that we
     *  have a waitset, manually trigger readiness.
     */
    if (_signalled && _activeWaitSets.size() == 1) {
      waitset.signalReadReadinessFromUserspace(
          reinterpret_cast<EventLoopSource::Handle>(this));
    }
  });
}

InProcessTrivialSource::~InProcessTrivialSource() {
  RL_ASSERT(_activeWaitSets.size() == 0);
}

}  // namespace core
}  // namespace rl

#endif  // RL_WAITSET == RL_WAITSET_INPROCESS
