// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef RADAR_CORE_SHAREDMEMORY_
#define RADAR_CORE_SHAREDMEMORY_

#include <Core/Macros.h>
#include <string>

namespace rl {
namespace core {

class SharedMemory {
 public:
  using Handle = int;

  /**
   *  Create a reference to shared memory of the given size
   *
   *  @param size the size of the shared memory
   */
  SharedMemory(size_t size);

  SharedMemory(Handle handle, bool assumeOwnership);

  ~SharedMemory();

  /**
   *  Cleanup the shared memory reference
   */
  void cleanup();

  /**
   *  Returns if the shared memory reference is ready for use
   *
   *  @return ready status of the shared memory
   */
  bool isReady() const;

  /**
   *  Returns the address of the shared memory reference
   *
   *  @return the shared memory reference address
   */
  uint8_t* address() const;

  /**
   *  Returns the size of the shared memory reference
   *
   *  @return the size of the shared memory reference
   */
  size_t size() const;

  /**
   *  Return the raw handle to the shared memory reference
   *
   *  @return the shared memory handle
   */
  Handle handle() const;

 private:
  Handle _handle;
  size_t _size;
  uint8_t* _address;
  bool _ready;
  bool _assumeOwnership;

  RL_DISALLOW_COPY_AND_ASSIGN(SharedMemory);
};

}  // namespace core
}  // namespace rl

#endif  // RADAR_CORE_SHAREDMEMORY_
