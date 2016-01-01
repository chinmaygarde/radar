// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <Core/Allocation.h>

namespace rl {
namespace core {

Allocation::Allocation() : _allocation(nullptr), _size(0) {}

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
  auto newAllocation =
      reinterpret_cast<uint8_t*>(realloc(_allocation, newSize * sizeof(char)));

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

bool Allocation::isReady() const {
  return _allocation != nullptr;
}

}  // namespace core
}  // namespace rl
