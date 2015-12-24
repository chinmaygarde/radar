// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <Core/Config.h>

#if RL_WAITSET == RL_WAITSET_EPOLL

#include <Core/Utilities.h>

#include "EPollWaitSet.h"

#include <sys/epoll.h>
#include <unistd.h>

namespace rl {
namespace core {

EPollWaitSet::EPollWaitSet() : _handle(-1) {
  _handle = RL_TEMP_FAILURE_RETRY(::epoll_create(1 /* unused */));
  RL_ASSERT(_handle != -1);
}

EPollWaitSet::~EPollWaitSet() {
  RL_CHECK(::close(_handle));
}

EventLoopSource* EPollWaitSet::wait(ClockDurationNano timeout) {
  struct epoll_event event = {0};

  int val = RL_TEMP_FAILURE_RETRY(
      ::epoll_wait(_handle, &event, 1, ToUnixTimeoutMS(timeout)));

  RL_ASSERT(val != -1);

  return val == 0 ? nullptr : static_cast<EventLoopSource*>(event.data.ptr);
}

WaitSet::Handle EPollWaitSet::handle() const {
  return _handle;
}

}  // namespace core
}  // namespace rl

#endif  // RL_WAITSET == RL_WAITSET_EPOLL
