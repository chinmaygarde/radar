// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <Shell/Shell.h>

#include <Compositor/Compositor.h>
#include <Host/Host.h>
#include <Interface/Interface.h>

// For setname_np. Can be made portable.
#include <pthread/pthread.h>

#include <cassert>

namespace rl {

Shell::Shell(std::shared_ptr<RenderSurface> surface,
             std::weak_ptr<InterfaceDelegate> delegate)
    : _hostThread(),
      _host(),
      _compositorThread(),
      _compositor(surface),
      _interfaceThread(),
      _interface(delegate, _compositor.patchChannel()) {
  Time::LoggingBootTime();
  attachHostOnCurrentThread();
}

void Shell::attachHostOnCurrentThread() {
  Latch readyLatch(3);

  pthread_setname_np("rl.host");
  _host.run(readyLatch);

  _compositorThread = std::move(std::thread([&]() {
    pthread_setname_np("rl.compositor");
    _compositor.run(readyLatch);
  }));

  _interfaceThread = std::move(std::thread([&]() {
    pthread_setname_np("rl.interface");
    _interface.run(readyLatch);
  }));

  readyLatch.wait();
}

Compositor& Shell::compositor() {
  return _compositor;
}

Interface& Shell::interface() {
  return _interface;
}

Host& Shell::host() {
  return _host;
}

void Shell::shutdown() {
  {
    /*
     *  We provide latches to the subsystems to count down on because they may
     *  be managing their own thread pools and other resources. We want those to
     *  wind down as well as. Once that happens, join the threads hosting the
     *  subsystems.
     */
    AutoLatch shutdownLatch(3);
    _host.shutdown(shutdownLatch);
    _compositor.shutdown(shutdownLatch);
    _interface.shutdown(shutdownLatch);
  }

  if (_compositorThread.joinable()) {
    _compositorThread.join();
  }

  if (_interfaceThread.joinable()) {
    _interfaceThread.join();
  }

  if (_hostThread.joinable()) {
    _hostThread.join();
  }
}

}  // namespace rl
