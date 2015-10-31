// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef RADARLOVE_CORE_LATCH_
#define RADARLOVE_CORE_LATCH_

#include <Core/Macros.h>
#include <atomic>
#include <condition_variable>

namespace rl {
namespace core {

/**
 *  A simple countdown latch used as a thread synchronization primitive
 */
class Latch {
 public:
  /**
   *  Create a latch with the specified count
   */
  explicit Latch(unsigned int count);

  /**
   *  Blocks the current thread till the count reaches zero. If the count is
   *  already zero, this opertation is a no-op
   */
  void wait();

  /**
   *  Decrement the count of the latch
   */
  void countDown();

 private:
  std::atomic_int _count;
  std::condition_variable _condition;
  std::mutex _lock;

  RL_DISALLOW_COPY_AND_ASSIGN(Latch);
};

/**
 *  A scoped latch. Performs an implicit wait when it goes out of scope.
 */
class AutoLatch : public Latch {
 public:
  AutoLatch(unsigned int count);
  ~AutoLatch();

 private:
  RL_DISALLOW_COPY_AND_ASSIGN(AutoLatch);
};

}  // namespace core
}  // namespace rl

#endif  // RADARLOVE_CORE_LATCH_
