/*
 *  This source file is part of the Radar project.
 *  Licensed under the MIT License. See LICENSE file for details.
 */

#include <Core/Utilities.h>
#include <Interface/Interface.h>
#include <Interface/InterfaceDelegate.h>
#include <Shell/DefaultInterface.h>

namespace rl {
namespace shell {

std::unique_ptr<interface::InterfaceDelegate> CreateDefaultInterface() {
  /*
   *  The default interface just creates a blank surface with a dark background.
   */
  auto interface = std::make_unique<interface::DefaultInterfaceDelegate>();
  interface->setInterfaceDidFinishLaunching(
      [](interface::Interface& interface) -> void {
        auto& root = interface.rootEntity();
        root.setBackgroundColor({0.2, 0.2, 0.2, 1.0});
      });
  return std::move(interface);
}

}  // namespace shell
}  // namespace rl
