// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <Shell/Shell.h>

#include <Coordinator/Coordinator.h>
#include <Shell/Host.h>
#include <Core/TraceEvent.h>
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
    : _compositorThread(),
      _coordinator(surface, _host.touchEventChannel()),
      _attached(false) {
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
  _attached = true;

  RL_TRACE_INSTANT("ShellAttached");
}

void Shell::shutdown() {
  /*
   *  Make sure all managed interfaces are torn down before the host and
   *  coordinators are torn down.
   */
  teardownManagedInterfaces();

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

void Shell::registerManagedInterface(
    std::unique_ptr<interface::Interface>&& interface) {
  RL_ASSERT_MSG(_attached,
                "The shell must be attached to a host before managed "
                "interfaces can be registered with it");

  std::lock_guard<std::mutex> lock(_interfacesMutex);

  /*
   *  Make sure the interface is launched on a new managed thread.
   */
  core::Latch interfaceLatch(1);
  const auto interfaceCount = _managedInterfaces.size();
  auto interfaceReady = [&]() { interfaceLatch.countDown(); };
  std::thread interfaceThread([&] {
    std::string threadName =
        "rl.interface." + std::to_string(interfaceCount + 1);
    core::thread::SetName(threadName.c_str());
    interface->run(interfaceReady);
  });
  interfaceLatch.wait();

  /*
   *  Note the registration.
   */
  _managedInterfaces.emplace(std::move(interface), std::move(interfaceThread));
}

void Shell::teardownManagedInterfaces() {
  std::lock_guard<std::mutex> lock(_interfacesMutex);

  {
    /*
     *  Make sure all interface shut down gracefully.
     */
    core::AutoLatch shutdownLatch(_managedInterfaces.size());
    auto onShutdown = [&]() { shutdownLatch.countDown(); };
    for (auto& interface : _managedInterfaces) {
      interface.first->shutdown(onShutdown);
    }
  }

  /*
   *  Ensure the threads hosting all managed interfaces shut down.
   */
  for (auto& interface : _managedInterfaces) {
    auto& thread = interface.second;
    if (thread.joinable()) {
      thread.join();
    }
  }

  _managedInterfaces.clear();
}

void Shell::dispatchTouchEvents(const std::vector<event::TouchEvent>& events) {
  _host.touchEventChannel().sendTouchEvents(events);
}

void Shell::renderSurfaceWasSetup() {
  _coordinator.renderSurfaceWasSetup();
}

void Shell::renderSurfaceDidUpdateSize(const geom::Size& size) {
  _coordinator.renderSurfaceSizeUpdated(size);
}

void Shell::renderSurfaceWasTornDown() {
  _coordinator.renderSurfaceWasTornDown();
}

}  // namespace shell
}  // namespace rl
