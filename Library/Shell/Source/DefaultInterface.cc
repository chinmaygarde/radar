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
  return std::make_unique<interface::DefaultInterfaceDelegate>();
}

}  // namespace shell
}  // namespace rl
