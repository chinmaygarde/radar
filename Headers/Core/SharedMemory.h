// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef __RADARLOVE__SHAREDMEMORY__
#define __RADARLOVE__SHAREDMEMORY__

#include "Core/Base.h"
#include <string>

namespace rl {

class SharedMemory {
 public:
  using Handle = int;
  explicit SharedMemory(size_t size);
  ~SharedMemory();

  void cleanup();

  bool isReady() const { return _ready; }

  void* address() const { return _address; }

  size_t size() const { return _size; }

  Handle handle() const { return _handle; }

 private:
  Handle _handle;
  size_t _size;
  void* _address;

  bool _ready;

  DISALLOW_COPY_AND_ASSIGN(SharedMemory);
};
}

#endif /* defined(__RADARLOVE__SHAREDMEMORY__) */
