// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef RADAR_SHELL_HOST_
#define RADAR_SHELL_HOST_

#include <Core/BootstrapServer.h>
#include <Core/Macros.h>
#include <Event/TouchEventChannel.h>

namespace rl {
namespace shell {

class Host {
 public:
  Host();

  /**
   *  Run the host access subsystem
   *
   *  @param onReady the callback to be invoked when the host is ready
   */
  void run(std::function<void()> onReady = nullptr);

  /**
   *  Gracefully shutdown the subsystem
   *
   *  @param onShutdown the callback to be invoked when the subsystem shuts down
   */
  void shutdown(std::function<void()> onShutdown = nullptr);

  event::TouchEventChannel& touchEventChannel();

 private:
  core::BootstrapServer _bootstrapServer;
  event::TouchEventChannel _touchEventChannel;

  RL_DISALLOW_COPY_AND_ASSIGN(Host);
};

}  // namespace shell
}  // namespace rl

#endif  // RADAR_SHELL_HOST_
