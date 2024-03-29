/*
 *  This source file is part of the Radar project.
 *  Licensed under the MIT License. See LICENSE file for details.
 */

#include "Config.h"

#if RL_WAITSET == RL_WAITSET_INPROCESS

#include <Core/WaitSet.h>
#include "InProcessTrivialSource.h"

namespace rl {
namespace core {

InProcessTrivialSource::InProcessTrivialSource()
    : EventLoopSource(), _signalled(false) {
  setHandlesProvider([&] {
    auto handle = reinterpret_cast<EventLoopSource::Handle>(this);
    return EventLoopSource::Handles(handle, handle);
  });

  setWriter([&](Handle) {
    std::lock_guard<std::mutex> lock(_activeWaitSetsMutex);
    _signalled = true;
    for (const auto& waitset : _activeWaitSets) {
      waitset->signalReadReadinessFromUserspace(
          {reinterpret_cast<EventLoopSource::Handle>(this)});
    }
    return IOResult::Success;
  });

  setReader([&](Handle) {
    std::lock_guard<std::mutex> lock(_activeWaitSetsMutex);
    _signalled = false;
    return IOResult::Success;
  });

  setCustomWaitSetUpdateHandler([&](EventLoopSource&, WaitSet& waitset, Handle,
                                    bool adding) {
    auto& inprocessWaitSet = static_cast<InProcessWaitSet&>(waitset.provider());

    std::lock_guard<std::mutex> lock(_activeWaitSetsMutex);

    if (adding) {
      _activeWaitSets.insert(&inprocessWaitSet);
    } else {
      _activeWaitSets.erase(&inprocessWaitSet);
    }

    /*
     *  The source could have been signalled before being added to the
     *  waitset. In this case, the writer failed to notify any waitsets. Now
     *  that we have a waitset, manually trigger readiness.
     */
    if (_signalled && _activeWaitSets.size() == 1) {
      inprocessWaitSet.signalReadReadinessFromUserspace(
          {reinterpret_cast<EventLoopSource::Handle>(this)});
    }
  });
}

InProcessTrivialSource::~InProcessTrivialSource() {
  RL_ASSERT(_activeWaitSets.size() == 0);
}

}  // namespace core
}  // namespace rl

#endif  // RL_WAITSET == RL_WAITSET_INPROCESS
