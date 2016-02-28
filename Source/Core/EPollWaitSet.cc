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

WaitSet::Result EPollWaitSet::wait(ClockDurationNano timeout) {
  struct epoll_event event = {};

  int val = RL_TEMP_FAILURE_RETRY(
      ::epoll_wait(_handle, &event, 1, ToUnixTimeoutMS(timeout)));

  RL_ASSERT(val != -1);

  switch (val) {
    case 0:
      /*
       *  A zero return from an epoll_wait indicates the expiry of a timeout.
       */
      return WaitSet::Result(WaitSet::WakeReason::Timeout, nullptr);
    case 1: {
      /*
       *  The only indicated item woke up. Check if it was because of a read or
       *  error.
       */
      auto result = WaitSet::WakeReason::ReadAvailable;
      if (event.events & (EPOLLERR | EPOLLHUP)) {
        /*
         *  In case of error or hangups, the source has awoken for termination.
         */
        result = WaitSet::WakeReason::Error;
      }
      return WaitSet::Result(result,
                             static_cast<EventLoopSource*>(event.data.ptr));
    }
    default:
      break;
  }

  return WaitSet::Result(WaitSet::WakeReason::Error, nullptr);
}

WaitSet::Handle EPollWaitSet::handle() const {
  return _handle;
}

}  // namespace core
}  // namespace rl

#endif  // RL_WAITSET == RL_WAITSET_EPOLL
