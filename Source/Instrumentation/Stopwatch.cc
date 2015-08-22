// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <Instrumentation/Stopwatch.h>

namespace rl {

Stopwatch::Stopwatch() : _start(0) {
}

void Stopwatch::start() {
  _start = Time::Current();
}

void Stopwatch::stop() {
  Time::Current() - _start;
}

}  // namespace rl
