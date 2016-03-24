// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "WaitSetProvider.h"

#include <Core/EventLoopSource.h>

namespace rl {
namespace core {

void WaitSetProvider::updateSource(WaitSet& waitset,
                                   EventLoopSource& source,
                                   bool addedOrRemoved) {
  source.updateInWaitSet(waitset, addedOrRemoved);
}

WaitSetProvider::~WaitSetProvider() {}

}  // namespace core
}  // namespace rl
