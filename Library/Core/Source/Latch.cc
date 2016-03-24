// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <Core/Latch.h>

namespace rl {
namespace core {

Latch::Latch(size_t count) : _count(count) {}

void Latch::wait() {
  if (_count > 0) {
    std::unique_lock<std::mutex> lock(_conditionMutex);
    _condition.wait(lock, [&] { return _count == 0; });
  }
}

void Latch::countDown() {
  if (--_count == 0) {
    _condition.notify_all();
  }
}

AutoLatch::AutoLatch(size_t count) : Latch(count) {}

AutoLatch::~AutoLatch() {
  wait();
}

}  // namespace core
}  // namespace rl