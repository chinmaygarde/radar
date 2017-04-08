/*
 *  This source file is part of the Radar project.
 *  Licensed under the MIT License. See LICENSE file for details.
 */

#pragma once

#include <Core/Macros.h>
#include <Interface/InterfaceDelegate.h>

namespace rl {
namespace shell {

std::unique_ptr<interface::InterfaceDelegate> CreateDefaultInterface();

}  // namespace shell
}  // namespace rl
