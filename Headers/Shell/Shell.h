// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef __RADARLOVE_SHELL_SHELL__
#define __RADARLOVE_SHELL_SHELL__

#include <thread>
#include <Core/Core.h>
#include "Compositor/Compositor.h"
#include "Compositor/RenderSurface.h"
#include "Host/Host.h"
#include "Interface/Interface.h"

namespace rl {
class Shell {
 public:
  Shell(std::shared_ptr<RenderSurface> surface,
        std::weak_ptr<InterfaceDelegate> delegate);

  /**
   *  Get a reference to the compositor
   *
   *  @return the compositor
   */
  Compositor& compositor();

  /**
   *  Get a reference to the interface
   *
   *  @return the interface
   */
  Interface& interface();

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
  bool _attached;

  /*
   *  Host variables
   */
  std::thread _hostThread;
  Host _host;

  /**
   *  Compositor variables
   */
  std::thread _compositorThread;
  Compositor _compositor;

  /*
   *  Interface variables
   */
  std::thread _interfaceThread;
  Interface _interface;

  void attachHostOnCurrentThread();

  DISALLOW_COPY_AND_ASSIGN(Shell);
};
}

#endif /* defined(__RADARLOVE_SHELL_SHELL__) */
