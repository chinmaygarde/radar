// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef RADARLOVE_CORE_INPROCESSTIMERSOURCE_H_
#define RADARLOVE_CORE_INPROCESSTIMERSOURCE_H_

#include <Core/Config.h>

#if RL_WAITSET == RL_WAITSET_INPROCESS

#include <Core/EventLoopSource.h>

#include <unordered_set>

namespace rl {
namespace core {

class InProcessTimerSource : public EventLoopSource {
 public:
  explicit InProcessTimerSource(ClockDurationNano repeatInterval);
  ~InProcessTimerSource();

 private:
  std::unordered_set<WaitSet*> _activeWaitSets;

  RL_DISALLOW_COPY_AND_ASSIGN(InProcessTimerSource);
};

}  // namespace core
}  // namespace rl

#endif  // RL_WAITSET == RL_WAITSET_INPROCESS

#endif  // RADARLOVE_CORE_INPROCESSTIMERSOURCE_H_
