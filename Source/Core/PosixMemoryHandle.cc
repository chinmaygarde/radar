// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <Core/Config.h>

#if RL_SHMEM == RL_SHMEM_POSIX

#include <Core/SharedMemoryHandle.h>

#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/types.h>

#include <sstream>

namespace rl {
namespace core {

static const int SharedMemoryTempHandleMaxRetries = 25;

static std::string SharedMemory_RandomFileName() {
  static uint32_t kSeed = 4;
  auto random = rand_r(&kSeed) % RAND_MAX;

  std::stringstream stream;

  stream << "/rl_SharedMemory" << random;

  return stream.str();
}

SharedMemory::Handle SharedMemoryHandleCreate(size_t size) {
  SharedMemory::Handle newHandle = -1;

  auto tempFile = SharedMemory_RandomFileName();

  int tries = 0;

  while (tries++ < SharedMemoryTempHandleMaxRetries) {
    newHandle = ::shm_open(tempFile.c_str(), O_RDWR | O_CREAT | O_EXCL,
                           S_IRUSR | S_IWUSR | O_CLOEXEC);

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

  /*
   *  Set the size of the shared memory
   */
  if (RL_TEMP_FAILURE_RETRY(::ftruncate(newHandle, size)) == -1) {
    RL_CHECK(::close(newHandle));
    return -1;
  }

  return newHandle;
}

}  // namespace core
}  // namespace rl

#endif  // RL_SHMEM == RL_SHMEM_POSIX
