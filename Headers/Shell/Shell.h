// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef __RADARLOVE_SHELL_SHELL__
#define __RADARLOVE_SHELL_SHELL__

#include <thread>
#include "Core/Base.h"
#include "Compositor/Compositor.h"
#include "Compositor/RenderSurface.h"
#include "Host/Host.h"
#include "Interface/Interface.h"

namespace rl {
class Shell {
 public:
  Shell(std::shared_ptr<RenderSurface> surface);

  /**
   *  Perform initialization of the shell on the current host thread.
   */
  void attachHostOnCurrentThread();

  /**
   *  Get a reference to the compositor
   *
   *  @return the compositor
   */
  Compositor& compositor() const;

  /**
   *  Get a reference to the interface
   *
   *  @return the interface
   */
  Interface& interface() const;

  /**
   *  Get a reference to the host
   *
   *  @return the host
   */
  Host& host() const;

 private:
  bool _attached;

  /*
   *  Host variables
   */
  std::thread _hostThread;
  std::unique_ptr<Host> _host;

  /**
   *  Compositor variables
   */
  std::thread _compositorThread;
  std::unique_ptr<Compositor> _compositor;

  /*
   *  Interface variables
   */
  std::thread _interfaceThread;
  std::unique_ptr<Interface> _interface;

  DISALLOW_COPY_AND_ASSIGN(Shell);
};
}

#endif /* defined(__RADARLOVE_SHELL_SHELL__) */
