// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef __RADARLOVE_CORE_KQUEUEWAITSET_H__
#define __RADARLOVE_CORE_KQUEUEWAITSET_H__

#include "WaitSetProvider.h"

namespace rl {

class KQueueWaitSet : public WaitSetProvider {
 public:
  KQueueWaitSet();
  ~KQueueWaitSet();

  EventLoopSource& wait() override;

  WaitSet::Handle handle() const override;

 private:
  using Handle = int;

  Handle _handle;

  RL_DISALLOW_COPY_AND_ASSIGN(KQueueWaitSet);
};

}  // namespace rl

#endif  // __RADARLOVE_CORE_KQUEUEWAITSET_H__
