// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <Core/Config.h>

#include <Core/Macros.h>
#include <Core/EventLoopSource.h>
#include <Core/Utilities.h>

namespace rl {

void EventLoopSource::updateInWaitSetHandleForSimpleRead(
    WaitSet::Handle waitsetHandle,
    bool shouldAdd) {
}

std::shared_ptr<EventLoopSource> EventLoopSource::Timer(
    std::chrono::nanoseconds repeatInterval) {
  return nullptr;
}

}  // namespace rl
