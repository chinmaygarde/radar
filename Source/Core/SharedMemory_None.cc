// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <Core/Config.h>

#if RL_OS_NACL || RL_OS_WINDOWS || RL_OS_ANDROID

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

SharedMemory::SharedMemory(size_t size)
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

#endif  // RL_OS_NACL
