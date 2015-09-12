// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <Core/Config.h>

#if !RL_OS_NACL

#include <Core/SharedMemory.h>
#include <Core/Utilities.h>

#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <sstream>

namespace rl {

static const int SharedMemoryTempHandleMaxRetries = 25;

static std::string SharedMemory_RandomFileName() {
  auto random = rand() % RAND_MAX;

  std::stringstream stream;

  stream << "/rl_SharedMemory" << random;

  return stream.str();
}

static SharedMemory::Handle SharedMemory_CreateBackingIfNecessary(
    SharedMemory::Handle handle) {
  /*
   *  A valid handle is already present. Nothing to do.
   */
  if (handle != -1) {
    return handle;
  }

  SharedMemory::Handle newHandle = -1;

  auto tempFile = SharedMemory_RandomFileName();

  int tries = 0;

  while (tries++ < SharedMemoryTempHandleMaxRetries) {
    newHandle = ::shm_open(tempFile.c_str(), O_RDWR | O_CREAT | O_EXCL,
                           S_IRUSR | S_IWUSR);

    if (newHandle == -1 && errno == EEXIST) {
      /*
       *  The current handle already exists (the O_CREAT | O_EXCL
       *  check is atomic). Try a new file name.
       */
      tempFile = SharedMemory_RandomFileName();
      continue;
    }

    break;
  }

  /*
   *  We already have a file reference, unlink the
   *  reference by name.
   */
  if (newHandle != -1) {
    RL_CHECK(::shm_unlink(tempFile.c_str()));
  }

  return newHandle;
}

SharedMemory::SharedMemory(size_t size)
    : _handle(-1), _size(0), _address(nullptr), _ready(false) {
  /*
   *  Step 1: Create a handle to shared memory
   */
  _handle = SharedMemory_CreateBackingIfNecessary(-1);

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

  if (_handle == -1) {
    RL_CHECK(::close(_handle));
  }

  _handle = -1;
}

void SharedMemory::cleanup() {
  if (!_ready) {
    return;
  }

  RL_CHECK(::munmap(_address, _size));
  RL_CHECK(::close(_handle));

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

#endif  // !RL_OS_NACL
