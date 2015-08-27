// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef __RADARLOVE_INSTRUMENTATION_STOPWATCH_H__
#define __RADARLOVE_INSTRUMENTATION_STOPWATCH_H__

#include <Core/Core.h>

namespace rl {

class Stopwatch {
 public:
  explicit Stopwatch();

  std::chrono::nanoseconds lastLap() const;
  std::chrono::nanoseconds currentLap() const;

  void start();
  void stop();

 private:
  std::chrono::nanoseconds _start;
  std::chrono::nanoseconds _lastLap;

  DISALLOW_COPY_AND_ASSIGN(Stopwatch);
};

}  // namespace rl

#endif  // __RADARLOVE_INSTRUMENTATION_STOPWATCH_H__
