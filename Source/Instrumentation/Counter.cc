// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <Instrumentation/Counter.h>

namespace rl {

Counter::Counter() : _count(0) {
}

size_t Counter::count() const {
  return _count;
}

void Counter::reset() {
  _count = 0;
}

void Counter::increment(size_t count) {
  _count += count;
}

}  // namespace rl
