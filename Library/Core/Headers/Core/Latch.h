// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#pragma once

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
  Latch(size_t count);

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
  std::atomic_size_t _count;
  std::condition_variable _condition;
  std::mutex _conditionMutex;

  RL_DISALLOW_COPY_AND_ASSIGN(Latch);
};

/**
 *  A scoped latch. Performs an implicit wait when it goes out of scope.
 */
class AutoLatch : public Latch {
 public:
  AutoLatch(size_t count);
  ~AutoLatch();

 private:
  RL_DISALLOW_COPY_AND_ASSIGN(AutoLatch);
};

class AutoCountdown {
 public:
  AutoCountdown(Latch& latch);

  ~AutoCountdown();

 private:
  Latch& _latch;

  RL_DISALLOW_COPY_AND_ASSIGN(AutoCountdown);
};

}  // namespace core
}  // namespace rl
