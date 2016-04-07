// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <Core/Config.h>
#include <Core/Allocation.h>
#include <Core/Message.h>

#if RL_OS_WINDOWS
#define bzero(buffer, length) (memset((buffer), '\0', (length)), (void)0)
#endif  // RL_OS_WINDOWS

namespace rl {
namespace core {

Allocation::Allocation() : _allocation(nullptr), _size(0) {}

Allocation::Allocation(uint8_t* bytes, size_t size)
    : _allocation(bytes), _size(size) {
  if (_size == 0) {
    free(_allocation);
    _allocation = nullptr;
  }

  if (_allocation == nullptr) {
    _size = 0;
  }
}

Allocation::~Allocation() {
  free(_allocation);
  _allocation = nullptr;
}

bool Allocation::resize(size_t newSize) {
  if (_size == newSize) {
    return _size != 0;
  }

  /*
   *  Like realloc, but with the following extensions:
   *  1: We want to follow the behavior of the `reallocf` routine from the BSDs
   *     when it comes to cleanups during reallocation failures.
   *  2: We want to guarantee zeroing out of the extended regions.
   */
  auto newAllocation = reinterpret_cast<uint8_t*>(
      realloc(_allocation, newSize * sizeof(uint8_t)));

  /*
   *  Handle failures during reallocation
   */
  if (newAllocation == nullptr && _allocation != nullptr && newSize != 0) {
    free(_allocation);
    _allocation = nullptr;
    _size = 0;
    return false;
  }

  /*
   *  Handle zeroing out of extended regions
   */
  if (newAllocation && newSize > _size) {
    bzero(newAllocation + _size, newSize - _size);
  }

  _allocation = newAllocation;
  _size = newSize;
  return _size != 0;
}

uint8_t* Allocation::data() {
  return _allocation;
}

uint8_t* Allocation::data() const {
  return _allocation;
}

size_t Allocation::size() const {
  return _size;
}

void Allocation::makeZero() {
  if (_size == 0) {
    return;
  }

  bzero(_allocation, _size);
}

bool Allocation::isReady() const {
  return _allocation != nullptr;
}

bool Allocation::serialize(Message& message) const {
  auto success = true;
  success &= message.encode(_size);
  auto allocation = message.encodeRaw<uint8_t>(_size);
  if (allocation == nullptr) {
    return false;
  }
  memmove(allocation, _allocation, _size);
  return success;
}

bool Allocation::deserialize(Message& message, Namespace* ns) {
  RL_ASSERT_MSG(!isReady(), "Can only deserialize into a fresh allocation");

  /*
   *  Decode the size of the allocation
   */
  bool success = message.decode(_size, ns);

  if (!success) {
    return false;
  }

  if (!resize(_size)) {
    return false;
  }

  auto allocation = message.decodeRaw<uint8_t>(_size);

  if (allocation == nullptr) {
    return false;
  }

  memmove(_allocation, allocation, _size);
  return true;
}

}  // namespace core
}  // namespace rl
