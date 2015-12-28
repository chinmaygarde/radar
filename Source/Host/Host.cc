// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <Host/Host.h>

namespace rl {

Host::Host() {}

void Host::run(std::function<void()> onReady) {
  if (onReady) {
    onReady();
  }
}

void Host::shutdown(std::function<void()> onShutdown) {
  if (onShutdown) {
    onShutdown();
  }
}

event::TouchEventChannel& Host::touchEventChannel() {
  return _touchEventChannel;
}

}  // namespace rl
