// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <Instrumentation/Stopwatch.h>

namespace rl {

Stopwatch::Stopwatch() : _last(0) {
  lap();
}

std::chrono::nanoseconds Stopwatch::lap() {
  return _last = Time::Current() - _last;
}

}  // namespace rl
