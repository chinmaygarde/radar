// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <Core/Config.h>

#if RL_OS_MAC || RL_OS_LINUX

#include <Core/SharedMemory.h>
#include <Core/Utilities.h>

#include "SharedMemoryHandle.h"

#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

namespace rl {

SharedMemory::SharedMemory(Handle handle, bool assumeOwnership)
    : _handle(handle),
      _size(0),
      _address(nullptr),
      _ready(false),
      _assumeOwnership(assumeOwnership) {
  /*
   *  We have a handle, figure out the size of the shared memory region
   */
  struct stat info = {0};
  if (fstat(handle, &info) != 0) {
    /*
     *  Must be able to successfully stat this mysterious descriptor
     */
    return;
  }

  _size = info.st_size;

  if (_size == 0) {
    return;
  }

  uint8_t* mapped = static_cast<uint8_t*>(
      ::mmap(nullptr, _size, PROT_READ | PROT_WRITE, MAP_SHARED, _handle, 0));

  if (mapped != MAP_FAILED) {
    _address = mapped;
    _ready = true;
  }
}

SharedMemory::SharedMemory(size_t size)
    : _handle(-1),
      _size(0),
      _address(nullptr),
      _ready(false),
      _assumeOwnership(true) {
  /*
   *  Step 1: Create a handle to shared memory
   */

  _handle = SharedMemoryHandleCreate();

  if (_handle == -1) {
    goto failure;
  }

  /*
   *  Step 2: Set the size of the shared memory
   */

  if (RL_TEMP_FAILURE_RETRY(::ftruncate(_handle, size)) == -1) {
    goto failure;
  }

  _size = size;

  /*
   *  Step 3: Memory map a region with the given handle
   */

  _address = static_cast<uint8_t*>(
      ::mmap(nullptr, size, PROT_READ | PROT_WRITE, MAP_SHARED, _handle, 0));

  if (_address == MAP_FAILED) {
    goto failure;
  }

  /*
   *  Jobs done!
   */

  _ready = true;

  return;

failure:
  RL_LOG_ERRNO();

  _ready = false;

  if (_address != nullptr) {
    RL_CHECK(::munmap(_address, size));
  }

  _size = 0;
  _address = nullptr;

  if (_handle != -1 && _assumeOwnership) {
    RL_CHECK(::close(_handle));
  }

  _handle = -1;
}

void SharedMemory::cleanup() {
  if (!_ready) {
    return;
  }

  if (_assumeOwnership) {
    RL_CHECK(::munmap(_address, _size));
    RL_CHECK(::close(_handle));
  }

  _handle = -1;
  _size = 0;
  _address = nullptr;
  _ready = false;
}

SharedMemory::~SharedMemory() {
  cleanup();
}

bool SharedMemory::isReady() const {
  return _ready;
}

uint8_t* SharedMemory::address() const {
  return _address;
}

size_t SharedMemory::size() const {
  return _size;
}

SharedMemory::Handle SharedMemory::handle() const {
  return _handle;
}

}  // namespace rl

#endif  // RL_OS_MAC || RL_OS_LINUX
