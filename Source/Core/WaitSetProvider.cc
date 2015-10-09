// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "WaitSetProvider.h"

#include <Core/EventLoopSource.h>

namespace rl {

void WaitSetProvider::updateSource(WaitSet& waitset,
                                   EventLoopSource& source,
                                   bool addedOrRemoved) {
  source.updateInWaitSet(waitset, addedOrRemoved);
}

void WaitSetProvider::signalReadReadinessFromUserspace(
    EventLoopSource::Handle writeHandle) {
  RL_ASSERT_MSG(
      false,
      "This platform does not allow signalling read readiness of sources "
      "from userspace. Only 'InProcess' Core variants support this!");
}

WaitSetProvider::~WaitSetProvider() {
}

}  // namespace rl
