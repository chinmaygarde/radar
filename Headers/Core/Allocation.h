// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef RADARLOVE_CORE_ALLOCATION_H_
#define RADARLOVE_CORE_ALLOCATION_H_

#include <Core/Macros.h>
#include <Core/Serializable.h>

namespace rl {
namespace core {

/**
 *  A simple memory allocation. Use this instead of `malloc`ing your own memory
 *  and managing its lifetime manually. As long as `isReady` method returns
 *  true, you are good to write into the allocation (till the next `resize`).
 *
 *  The following guarantees are provided by this wrapper:
 *  1: In case of reallocation failures due to memory constraints, the old
 *     allocation is freed and the `isReady` method returns false.
 *  2: All allocations, including resizes that cause extensions in the memory
 *     arena are zero'ed out
 *
 *  After each call to resize, you are responsible for fetching the new
 *  reference to the memory arena as the old one may have been invalidated. Use
 *  `resize` like you would treat a `realloc`
 *
 *  You are still responsible for writing within the bounds of the memory arena.
 *  If your program breaks because of this, please keep both pieces.
 */
class Allocation : public Serializable {
 public:
  /**
   *  A new zero sized allocation. This is not ready till the call to the first
   *  non zero resize
   */
  Allocation();

  ~Allocation();

  /**
   *  @return Get a pointer to the underlying memory arena
   */
  uint8_t* data();

  /**
   *  @return the size of the underlying allocation
   */
  size_t size() const;

  /**
   *  Resize the memory arena to the new value
   *
   *  @param size the new size of the allocation
   *
   *  @return if the resized arena is ready for writing into
   */
  bool resize(size_t size);

  /**
   *  @return if the allocation is ready for writing into
   */
  bool isReady() const;

  /**
   *  Zero out the current allocation
   */
  void makeZero();

  bool serialize(Message& message) const override;

  bool deserialize(Message& message, Namespace* ns) override;

 private:
  uint8_t* _allocation;
  size_t _size;

  RL_DISALLOW_COPY_AND_ASSIGN(Allocation);
};

}  // namespace core
}  // namespace rl

#endif  // RADARLOVE_CORE_ALLOCATION_H_
