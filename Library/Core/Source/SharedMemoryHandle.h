/*
 *  This source file is part of the Radar project.
 *  Licensed under the MIT License. See LICENSE file for details.
 */

#pragma once

#include <Core/SharedMemory.h>

namespace rl {
namespace core {

SharedMemory::Handle SharedMemoryHandleCreate(size_t size);

}  // namespace core
}  // namespace rl
