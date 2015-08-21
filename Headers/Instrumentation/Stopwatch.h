// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef __RADARLOVE_INSTRUMENTATION_STOPWATCH_H__
#define __RADARLOVE_INSTRUMENTATION_STOPWATCH_H__

#include <Core/Core.h>

namespace rl {

class Stopwatch {
 public:
  Stopwatch();

  std::chrono::nanoseconds lap();

 private:
  std::chrono::nanoseconds _last;

  DISALLOW_COPY_AND_ASSIGN(Stopwatch);
};

}  // namespace rl

#endif  // __RADARLOVE_INSTRUMENTATION_STOPWATCH_H__
