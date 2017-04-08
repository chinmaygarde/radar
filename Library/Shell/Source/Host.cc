/*
 *  This source file is part of the Radar project.
 *  Licensed under the MIT License. See LICENSE file for details.
 */

#include <Shell/Host.h>

namespace rl {
namespace shell {

Host::Host() {}

void Host::run(std::function<void()> onReady) {
  if (onReady) {
    onReady();
  }
}

void Host::shutdown(std::function<void()> onShutdown) {
  if (onShutdown) {
    onShutdown();
  }
}

event::TouchEventChannel& Host::touchEventChannel() {
  return _touchEventChannel;
}

}  // namespace shell
}  // namespace rl
