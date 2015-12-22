// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <Shell/Shell.h>

#include <Coordinator/Coordinator.h>
#include <Host/Host.h>
#include <Instrumentation/TraceEvent.h>
#include <Interface/Interface.h>

namespace rl {
namespace shell {

Shell::Shell(std::shared_ptr<coordinator::RenderSurface> surface,
             std::weak_ptr<interface::InterfaceDelegate>
                 delegate)
    : _compositorThread(),
      _coordinator(surface, _host.touchEventChannel()),
      _interface(delegate) {
  RL_TRACE_INSTANT("ShellInitialization");
  core::clock::LoggingClockDuration();
  attachHostOnCurrentThread();
}

void Shell::attachHostOnCurrentThread() {
  core::Latch readyLatch(3);

  core::thread::SetName("rl.host");
  _host.run(readyLatch);

  _compositorThread = std::move(std::thread([&]() {
    core::thread::SetName("rl.coordinator");
    _coordinator.run(readyLatch);
  }));

  _interfaceThread = std::move(std::thread([&]() {
    core::thread::SetName("rl.interface");
    _interface.run(readyLatch);
  }));

  readyLatch.wait();

  RL_TRACE_INSTANT("ShellAttached");
}

coordinator::Coordinator& Shell::coordinator() {
  return _coordinator;
}

interface::Interface& Shell::interface() {
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
    core::AutoLatch shutdownLatch(3);
    _host.shutdown(shutdownLatch);
    _coordinator.shutdown(shutdownLatch);
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

}  // namespace shell
}  // namespace rl
