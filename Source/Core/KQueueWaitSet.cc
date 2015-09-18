// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <Core/Config.h>

#if RL_WAITSET == RL_WAITSET_KQUEUE

#include "KQueueWaitSet.h"

#include <sys/event.h>
#include <unistd.h>

namespace rl {

KQueueWaitSet::KQueueWaitSet() : _handle(-1) {
  _handle = RL_TEMP_FAILURE_RETRY(::kqueue());
  RL_ASSERT(_handle != -1);
}

KQueueWaitSet::~KQueueWaitSet() {
  RL_CHECK(::close(_handle));
  _handle = -1;
}

EventLoopSource& KQueueWaitSet::wait() {
  struct kevent event = {0};

  int val =
      RL_TEMP_FAILURE_RETRY(::kevent(_handle, nullptr, 0, &event, 1, nullptr));

  RL_ASSERT(val == 1);
  RL_ASSERT(event.udata != nullptr);

  return *static_cast<EventLoopSource*>(event.udata);
}

WaitSet::Handle KQueueWaitSet::handle() const {
  return _handle;
}

}  // namespace rl

#endif  // RL_OS_MAC
