// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef RADARLOVE_CORE_INPROCESSTRIVIALSOURCE_H_
#define RADARLOVE_CORE_INPROCESSTRIVIALSOURCE_H_

#include <Core/Config.h>

#if RL_WAITSET == RL_WAITSET_INPROCESS

#include <Core/EventLoopSource.h>
#include <mutex>
#include <unordered_set>
#include "InProcessWaitSet.h"

namespace rl {
namespace core {

class InProcessTrivialSource : public EventLoopSource {
 public:
  InProcessTrivialSource();

  ~InProcessTrivialSource();

 private:
  std::mutex _activeWaitSetsMutex;
  std::unordered_set<InProcessWaitSet*> _activeWaitSets;
  bool _signalled;

  RL_DISALLOW_COPY_AND_ASSIGN(InProcessTrivialSource);
};

}  // namespace core
}  // namespace rl

#endif  // RL_WAITSET == RL_WAITSET_INPROCESS

#endif  // RADARLOVE_CORE_INPROCESSTRIVIALSOURCE_H_
