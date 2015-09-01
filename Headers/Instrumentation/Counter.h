// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef __RADARLOVE_INSTRUMENTATION_COUNTER_H__
#define __RADARLOVE_INSTRUMENTATION_COUNTER_H__

#include <Core/Core.h>

namespace rl {

class Counter {
 public:
  explicit Counter();

  size_t count() const;

  void reset(size_t count = 0);

  void increment(size_t count = 1);

 private:
  size_t _count;

  DISALLOW_COPY_AND_ASSIGN(Counter);
};

}  // namespace rl

#endif  // __RADARLOVE_INSTRUMENTATION_COUNTER_H__
