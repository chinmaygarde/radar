// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <Core/Config.h>

#if RL_OS_MAC

#include <Core/WaitSet.h>

#include <sys/event.h>
#include <unistd.h>

namespace rl {

WaitSet::Handle WaitSet::platformHandleCreate() {
  WaitSet::Handle handle = RL_TEMP_FAILURE_RETRY(::kqueue());

  RL_ASSERT(handle != -1);

  return handle;
}

EventLoopSource& WaitSet::platformHandleWait(WaitSet::Handle handle) {
  struct kevent event = {0};

  int val =
      RL_TEMP_FAILURE_RETRY(::kevent(handle, nullptr, 0, &event, 1, nullptr));

  RL_ASSERT(val == 1);
  RL_ASSERT(event.udata != nullptr);

  return *static_cast<EventLoopSource*>(event.udata);
}

void WaitSet::platformHandleDestory(WaitSet::Handle handle) {
  RL_CHECK(::close(handle));
}

}  // namespace rl

#endif  // RL_OS_MAC