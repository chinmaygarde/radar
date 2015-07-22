// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "Core/Latch.h"

using namespace rl;

Latch::Latch(unsigned int count) : _count(count), _condition(), _lock() {
}

void Latch::wait() {
  if (_count > 0) {
    std::unique_lock<std::mutex> lock(_lock);
    _condition.wait(lock, [&] { return _count == 0; });
    _lock.unlock();
  }
}

void Latch::countDown() {
  if (--_count == 0) {
    _condition.notify_all();
  }
}
