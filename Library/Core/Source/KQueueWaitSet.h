/*
 *  This source file is part of the Radar project.
 *  Licensed under the MIT License. See LICENSE file for details.
 */

#pragma once

#include "Config.h"

#if RL_WAITSET == RL_WAITSET_KQUEUE

#include "WaitSetProvider.h"

namespace rl {
namespace core {

class KQueueWaitSet : public WaitSetProvider {
 public:
  KQueueWaitSet();
  ~KQueueWaitSet();

  WaitSet::Result wait(ClockDurationNano timeout) override;

  WaitSet::Handle handle() const override;

 private:
  using Handle = int;

  Handle _handle;

  RL_DISALLOW_COPY_AND_ASSIGN(KQueueWaitSet);
};

}  // namespace core
}  // namespace rl

#endif  // RL_WAITSET == RL_WAITSET_KQUEUE
