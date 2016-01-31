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

std::unique_ptr<Shell> Shell::CreateWithCurrentThreadAsHost(
    std::shared_ptr<coordinator::RenderSurface> surface) {
  /*
   *  Any log messages will be relative to the first call to the logging
   *  duration. This is as good a spot as any to initialize the logging
   *  duration.
   */
  core::clock::LoggingClockDuration();

  /*
   *  Create an instance of the shell and attach to the current thread.
   *  This is a blocking operation and waits for the full initialization of
   *  all service threads.
   */
  auto shell = std::unique_ptr<Shell>(new Shell(std::move(surface)));
  shell->attachHostOnCurrentThread() /* blocking till completion */;

  return shell /* go forth and prosper */;
}

Shell::Shell(std::shared_ptr<coordinator::RenderSurface> surface)
    : _compositorThread(), _coordinator(surface, _host.touchEventChannel()) {
  RL_TRACE_INSTANT("ShellInitialization");
}

void Shell::attachHostOnCurrentThread() {
  core::Latch readyLatch(2);
  auto onReady = [&]() { readyLatch.countDown(); };

  core::thread::SetName("rl.host");
  _host.run(onReady);

  _compositorThread = std::thread([&]() {
    core::thread::SetName("rl.coordinator");
    _coordinator.run(onReady);
  });

  readyLatch.wait();

  RL_TRACE_INSTANT("ShellAttached");
}

coordinator::Coordinator& Shell::coordinator() {
  return _coordinator;
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
    core::AutoLatch shutdownLatch(2);
    auto onShutdown = [&]() { shutdownLatch.countDown(); };

    _host.shutdown(onShutdown);
    _coordinator.shutdown(onShutdown);
  }

  if (_compositorThread.joinable()) {
    _compositorThread.join();
  }

  if (_hostThread.joinable()) {
    _hostThread.join();
  }
}

}  // namespace shell
}  // namespace rl
