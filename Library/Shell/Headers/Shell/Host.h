/*
 *  This source file is part of the Radar project.
 *  Licensed under the MIT License. See LICENSE file for details.
 */

#pragma once

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
