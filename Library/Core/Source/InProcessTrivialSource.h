/*
 *  This source file is part of the Radar project.
 *  Licensed under the MIT License. See LICENSE file for details.
 */

#pragma once

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
