/*
 *  This source file is part of the Radar project.
 *  Licensed under the MIT License. See LICENSE file for details.
 */

#pragma once

#include <Coordinator/Coordinator.h>
#include <Coordinator/RenderSurface.h>
#include <Core/Macros.h>
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
  using ManagedInterfaces =
      std::unordered_map<std::unique_ptr<interface::Interface>, std::thread>;

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
  core::Mutex _managedInterfacesMutex;
  ManagedInterfaces _managedInterfaces RL_GUARDED_BY(_managedInterfacesMutex);

  Shell(std::shared_ptr<coordinator::RenderSurface> surface);

  void attachHostOnCurrentThread();

  void teardownManagedInterfaces();

  RL_DISALLOW_COPY_AND_ASSIGN(Shell);
};

}  // namespace shell
}  // namespace rl
