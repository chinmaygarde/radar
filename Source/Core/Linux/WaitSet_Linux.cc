// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "Core/WaitSet.h"
#include "Core/Utilities.h"

#include <sys/epoll.h>
#include <unistd.h>

namespace rl {

WaitSet::Handle WaitSet::platformHandleCreate() {
  WaitSet::Handle handle =
      RL_TEMP_FAILURE_RETRY(::epoll_create(1 /* unused */));

  RL_ASSERT(handle != -1);
  return handle;
}

LooperSource* WaitSet::platformHandleWait(WaitSet::Handle handle) {
  struct epoll_event event = {0};

  int val = RL_TEMP_FAILURE_RETRY(
      ::epoll_wait(handle, &event, 1, -1 /* infinite timeout */));

  RL_ASSERT(val == 1);

  return static_cast<LooperSource*>(event.data.ptr);
}

void WaitSet::platformHandleDestory(WaitSet::Handle handle) {
  RL_CHECK(::close(handle));
}

}  // namespace rl
