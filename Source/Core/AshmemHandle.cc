// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <Core/Config.h>

#if RL_SHMEM == RL_SHMEM_ASHMEM

#include "SharedMemoryHandle.h"

#include <sys/mman.h>
#include <fcntl.h>

namespace rl {
namespace core {

SharedMemory::Handle SharedMemoryHandleCreate() {
  RL_ASSERT(false);
  return -1;
}

}  // namespace core
}  // namespace rl

#endif  // RL_SHMEM == RL_SHMEM_ASHMEM
