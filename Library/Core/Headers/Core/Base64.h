/*
 *  This source file is part of the Radar project.
 *  Licensed under the MIT License. See LICENSE file for details.
 */

#pragma once

#include <Core/Allocation.h>
#include <Core/Macros.h>

namespace rl {
namespace core {

Allocation Base64Decode(const uint8_t* data);

}  // namespace core
}  // namespace rl
