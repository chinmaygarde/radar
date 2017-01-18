// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef RADAR_SHELL_SHELL_
#define RADAR_SHELL_SHELL_

#include <Coordinator/Coordinator.h>
#include <Coordinator/RenderSurface.h>
#include <Core/Macros.h>
#include <Core/Mutexed.h>
#include <Interface/Interface.h>
#include <Shell/Host.h>

#include <thread>
#include <unordered_map>

namespace rl {
namespace shell {

class Shell {
 public:
  static std::unique_ptr<Shell> CreateWithCurrentThreadAsHost(
      std::shared_ptr<coordinator::RenderSurface> surface);

  ~Shell();

  /**
   *  Blocking operation that gracefully shuts down all subsystems and makes
   *  sure all callbacks are sent.
   */
  void shutdown();

  /**
   *  Register an interface hosted within the process of the coordinator. This
   *  can only be used rarely since but is convenient for simple interfaces.
   *
   *  @param interface the interface to register.
   */
  bool registerManagedInterface(
      std::unique_ptr<interface::Interface> interface);

  /**
   *  Dispatch touch events to the coordinator.
   *
   *  @param events the events to dispatch.
   */
  void dispatchTouchEvents(const std::vector<event::TouchEvent>& events);

  void renderSurfaceWasSetup();

  void renderSurfaceDidUpdateSize(const geom::Size& size);

  void redrawCurrentFrameNow();

  void renderSurfaceWasTornDown();

 private:
  Shell(std::shared_ptr<coordinator::RenderSurface> surface);

  std::thread _hostThread;
  std::thread _compositorThread;
  Host _host;
  coordinator::Coordinator _coordinator;
  bool _attached;
  bool _wasShutDown;

  /*
   *  A guarded map between unique pointers to the interface and the threads
   *  these interfaces are being serviced on.
   */
  core::Mutexed<
      std::unordered_map<std::unique_ptr<interface::Interface>, std::thread>,
      std::mutex>
      _managedInterfaces;

  void attachHostOnCurrentThread();
  void teardownManagedInterfaces();

  RL_DISALLOW_COPY_AND_ASSIGN(Shell);
};

}  // namespace shell
}  // namespace rl

#endif  // RADAR_SHELL_SHELL_
