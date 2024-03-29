/*
 *  This source file is part of the Radar project.
 *  Licensed under the MIT License. See LICENSE file for details.
 */

#include "Config.h"

#if RL_SHMEM == RL_SHMEM_DISABLED

#include <Core/SharedMemory.h>

namespace rl {
namespace core {

SharedMemory::SharedMemory(Handle handle, bool assumeOwnership)
    : _handle(handle),
      _size(0),
      _address(nullptr),
      _ready(false),
      _assumeOwnership(assumeOwnership) {
  RL_ASSERT(false);
}

SharedMemory::SharedMemory(size_t)
    : _handle(-1),
      _size(0),
      _address(nullptr),
      _ready(false),
      _assumeOwnership(true) {
  RL_ASSERT(false);
}

void SharedMemory::cleanup() {
  RL_ASSERT(false);
}

SharedMemory::~SharedMemory() {
  cleanup();
}

bool SharedMemory::isReady() const {
  RL_ASSERT(false);
  return false;
}

uint8_t* SharedMemory::address() const {
  RL_ASSERT(false);
  return nullptr;
}

size_t SharedMemory::size() const {
  RL_ASSERT(false);
  return 0;
}

SharedMemory::Handle SharedMemory::handle() const {
  RL_ASSERT(false);
  return -1;
}

}  // namespace core
}  // namespace rl

#endif  // RL_SHMEM == RL_SHMEM_DISABLED
