// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <Core/Config.h>

#if RL_OS_NACL

#include <Core/Macros.h>
#include <Core/EventLoopSource.h>
#include <Core/Utilities.h>

#include "InProcessTrivialSource.h"

namespace rl {

void EventLoopSource::updateInWaitSetForSimpleRead(WaitSet& waitset,
                                                   bool shouldAdd) {
}

std::shared_ptr<EventLoopSource> EventLoopSource::Timer(
    std::chrono::nanoseconds repeatInterval) {
  return nullptr;
}

std::shared_ptr<EventLoopSource> EventLoopSource::Trivial() {
  return std::make_shared<InProcessTrivialSource>();
}

}  // namespace rl

#endif  // RL_OS_NACL
