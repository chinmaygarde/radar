// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef RADAR_INSTRUMENTATION_COUNTER_H_
#define RADAR_INSTRUMENTATION_COUNTER_H_

#include <Core/Macros.h>

namespace rl {
namespace instrumentation {

class Counter {
 public:
  Counter();

  size_t count() const;

  void reset(size_t count = 0);

  void increment(size_t count = 1);

 private:
  size_t _count;

  RL_DISALLOW_COPY_AND_ASSIGN(Counter);
};

}  // namespace instrumentation
}  // namespace rl

#endif  // RADAR_INSTRUMENTATION_COUNTER_H_
