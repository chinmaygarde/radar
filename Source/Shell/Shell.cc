// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "Shell/Shell.h"

#include "Compositor/Compositor.h"
#include "Host/Host.h"
#include "Interface/Interface.h"

// For setname_np. Can be made portable.
#include <pthread/pthread.h>

#include <cassert>

using namespace rl;

Shell::Shell(std::shared_ptr<RenderSurface> surface)
    : _attached(false),
      _hostThread(),
      _host(Utils::make_unique<Host>()),
      _compositorThread(),
      _compositor(Utils::make_unique<Compositor>(surface)),
      _interfaceThread(),
      _interface(Utils::make_unique<Interface>()) {
}

void Shell::attachHostOnCurrentThread() {
  if (_attached) {
    return;
  }

  _attached = true;

  Latch readyLatch(3);

  pthread_setname_np("rl.host");
  _host->run(readyLatch);

  _compositorThread = std::move(std::thread([&]() {
    pthread_setname_np("rl.compositor");
    _compositor->run(readyLatch);
  }));

  _interfaceThread = std::move(std::thread([&]() {
    pthread_setname_np("rl.interface");
    _interface->run(readyLatch);
  }));

  readyLatch.wait();
}
