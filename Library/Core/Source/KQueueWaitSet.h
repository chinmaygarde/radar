// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

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
