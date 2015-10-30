// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <Host/Host.h>

namespace rl {

Host::Host() {
}

void Host::run(Latch& readyLatch) {
  readyLatch.countDown();
}

void Host::shutdown(Latch& shutdownLatch) {
  shutdownLatch.countDown();
}

event::TouchEventChannel& Host::touchEventChannel() {
  return _touchEventChannel;
}

}  // namespace rl
