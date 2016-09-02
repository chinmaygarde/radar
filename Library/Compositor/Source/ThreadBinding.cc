// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <Compositor/ThreadBinding.h>

namespace rl {
namespace compositor {

ThreadBinding::ThreadBinding(bool bind) : _bound(bind) {}

ThreadBinding::~ThreadBinding() {
  if (_bound) {
    RL_ASSERT_MSG(
        _threadID == std::this_thread::get_id(),
        "A resource was collected on a thread other than the thread on "
        "which it was first bound.");
  }
}

void ThreadBinding::bind() {
  if (_bound) {
    return;
  }

  _threadID = std::this_thread::get_id();
  _bound = true;
}

void ThreadBinding::unbind() {
  if (!_bound) {
    return;
  }

  _bound = false;
}

bool ThreadBinding::isBound() const {
  return _bound;
}

}  // namespace compositor
}  // namespace rl
