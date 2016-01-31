// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef RADARLOVE_SHELL_SHELL_
#define RADARLOVE_SHELL_SHELL_

#include <Coordinator/Coordinator.h>
#include <Coordinator/RenderSurface.h>
#include <Core/Core.h>
#include <Host/Host.h>
#include <Interface/Interface.h>

#include <thread>

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

 private:
  Shell(std::shared_ptr<coordinator::RenderSurface> surface);

  /*
   *  Host variables
   */
  std::thread _hostThread;
  Host _host;

  /**
   *  Compositor variables
   */
  std::thread _compositorThread;
  coordinator::Coordinator _coordinator;

  void attachHostOnCurrentThread();

  RL_DISALLOW_COPY_AND_ASSIGN(Shell);
};

}  // namespace shell
}  // namespace rl

#endif  // RADARLOVE_SHELL_SHELL_
