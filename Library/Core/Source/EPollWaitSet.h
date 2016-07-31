// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef RADARLOVE_CORE_EPOLLWAITSET_H_
#define RADARLOVE_CORE_EPOLLWAITSET_H_

#include "Config.h"

#if RL_WAITSET == RL_WAITSET_EPOLL

#include <Core/Macros.h>
#include "WaitSetProvider.h"

namespace rl {
namespace core {

class EPollWaitSet : public WaitSetProvider {
 public:
  EPollWaitSet();

  ~EPollWaitSet();

  WaitSet::Result wait(ClockDurationNano timeout) override;

  WaitSet::Handle handle() const override;

 private:
  using Handle = int;

  Handle _handle;

  RL_DISALLOW_COPY_AND_ASSIGN(EPollWaitSet);
};

}  // namespace core
}  // namespace rl

#endif  // RL_WAITSET == RL_WAITSET_EPOLL

#endif  // RADARLOVE_CORE_EPOLLWAITSET_H_
