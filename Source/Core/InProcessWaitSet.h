// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef __RADARLOVE_CORE_INPROCESSWAITSET_H__
#define __RADARLOVE_CORE_INPROCESSWAITSET_H__

#include <Core/Macros.h>
#include <Core/WaitSetProvider.h>

#include <unordered_map>
#include <unordered_set>
#include <condition_variable>

namespace rl {

class InProcessWaitSet : public WaitSetProvider {
 public:
  InProcessWaitSet();
  ~InProcessWaitSet();

  static constexpr EventLoopSource::Handle TimerHandle() {
    return std::numeric_limits<EventLoopSource::Handle>::max();
  }

  EventLoopSource& wait() override;
  WaitSet::Handle handle() const override;
  void signalReadReadinessFromUserspace(
      EventLoopSource::Handle writeHandle) override;
  void updateSource(WaitSet& waitset,
                    EventLoopSource& source,
                    bool addedOrRemoved) override;

 private:
  using WriteHandleSourcesMap =
      std::unordered_map<EventLoopSource::Handle, EventLoopSource*>;

  std::mutex _lock;
  std::condition_variable _conditionVariable;
  WriteHandleSourcesMap _watchedSources;
  std::unordered_set<EventLoopSource*> _watchedTimers;
  std::unordered_set<EventLoopSource*> _readySources;

  RL_DISALLOW_COPY_AND_ASSIGN(InProcessWaitSet);
};

}  // namespace rl

#endif  // __RADARLOVE_CORE_INPROCESSWAITSET_H__
