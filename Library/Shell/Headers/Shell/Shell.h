// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef RADARLOVE_SHELL_SHELL_
#define RADARLOVE_SHELL_SHELL_

#include <Coordinator/Coordinator.h>
#include <Coordinator/RenderSurface.h>
#include <Core/Core.h>
#include <Shell/Host.h>
#include <Interface/Interface.h>

#include <thread>
#include <mutex>
#include <unordered_map>

namespace rl {
namespace shell {

class Shell {
 public:
  static std::unique_ptr<Shell> CreateWithCurrentThreadAsHost(
      std::shared_ptr<coordinator::RenderSurface> surface);

  /**
   *  Get a reference to the coordinator
   *
   *  @return the coordinator
   */
  coordinator::Coordinator& coordinator();

  /**
   *  Get a reference to the host
   *
   *  @return the host
   */
  Host& host();

  /**
   *  Blocking operation that gracefully shuts down all subsystems and makes
   *  sure all callbacks are sent.
   */
  void shutdown();

  void registerManagedInterface(
      std::unique_ptr<interface::Interface>&& interface);

 private:
  Shell(std::shared_ptr<coordinator::RenderSurface> surface);

  std::thread _hostThread;
  std::thread _compositorThread;
  Host _host;
  coordinator::Coordinator _coordinator;
  bool _attached;
  std::mutex _interfacesMutex;
  std::unordered_map<std::unique_ptr<interface::Interface>, std::thread>
      _managedInterfaces;

  void attachHostOnCurrentThread();
  void teardownManagedInterfaces();

  RL_DISALLOW_COPY_AND_ASSIGN(Shell);
};

}  // namespace shell
}  // namespace rl

#endif  // RADARLOVE_SHELL_SHELL_
