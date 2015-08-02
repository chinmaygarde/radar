// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef __RADARLOVE_CORE_LATCH__
#define __RADARLOVE_CORE_LATCH__

#include "Core/Base.h"
#include <atomic>
#include <condition_variable>

namespace rl {
/**
 *  A simple countdown latch used as a thread synchronization primitive
 */
class Latch {
 public:
  /**
   *  Create a latch with the specified count
   */
  Latch(unsigned int count);

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

  DISALLOW_COPY_AND_ASSIGN(Latch);
};
}

#endif /* defined(__RADARLOVE_CORE_LATCH__) */
