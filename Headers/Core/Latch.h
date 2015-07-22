// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef __RADARLOVE_CORE_LATCH__
#define __RADARLOVE_CORE_LATCH__

#include "Core/Base.h"
#include <atomic>
#include <condition_variable>

namespace rl {
class Latch {
 public:
  Latch(unsigned int count);
  void wait();
  void countDown();

 private:
  std::atomic_int _count;
  std::condition_variable _condition;
  std::mutex _lock;
  DISALLOW_COPY_AND_ASSIGN(Latch);
};
}

#endif /* defined(__RADARLOVE_CORE_LATCH__) */
