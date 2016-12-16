// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <Core/Allocation.h>

#include <Core/Message.h>

#include "Config.h"

#if RL_OS_WINDOWS
#define bzero(buffer, length) (memset((buffer), '\0', (length)), (void)0)
#endif  // RL_OS_WINDOWS

namespace rl {
namespace core {

Allocation::Allocation() : _allocation(nullptr), _size(0) {}

static uint8_t* AllocationCopy(const uint8_t* bytes, size_t size) {
  if (bytes == nullptr || size == 0) {
    return nullptr;
  }

  auto allocation = static_cast<uint8_t*>(::malloc(size));

  if (allocation == nullptr) {
    return nullptr;
  }

  ::memcpy(allocation, bytes, size);
  return allocation;
}

Allocation::Allocation(uint8_t* bytes, size_t size, bool copy)
    : _allocation(copy ? AllocationCopy(bytes, size) : bytes), _size(size) {
  if (_size == 0) {
    free(_allocation);
    _allocation = nullptr;
  }

  if (_allocation == nullptr) {
    _size = 0;
  }
}

Allocation::Allocation(const uint8_t* bytes, size_t size)
    : Allocation(const_cast<uint8_t*>(bytes), size, true) {}

Allocation::Allocation(Allocation&& other)
    : _allocation(other._allocation), _size(other._size) {
  other._allocation = nullptr;
  other._size = 0;
}

Allocation& Allocation::operator=(Allocation&& other) {
  _allocation = other._allocation;
  _size = other._size;

  other._allocation = nullptr;
  other._size = 0;

  return *this;
}

Allocation::~Allocation() {
  if (_allocation != nullptr) {
    free(_allocation);
    _allocation = nullptr;
  }
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
  /*
   *  Encode the size.
   */
  RL_RETURN_IF_FALSE(message.encode(_size));

  /*
   *  Encode the message.
   */
  auto allocation = message.encodeRaw<uint8_t>(_size);
  if (allocation == nullptr) {
    return false;
  }
  memmove(allocation, _allocation, _size);

  return true;
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
