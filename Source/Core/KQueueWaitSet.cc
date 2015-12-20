// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <Core/Config.h>

#if RL_WAITSET == RL_WAITSET_KQUEUE

#include "KQueueWaitSet.h"

#include <sys/event.h>
#include <unistd.h>

#ifndef NSEC_PER_SEC
#define NSEC_PER_SEC 1000000000
#endif

namespace rl {
namespace core {

KQueueWaitSet::KQueueWaitSet() : _handle(-1) {
  _handle = RL_TEMP_FAILURE_RETRY(::kqueue());
  RL_ASSERT(_handle != -1);
}

KQueueWaitSet::~KQueueWaitSet() {
  RL_CHECK(::close(_handle));
  _handle = -1;
}

EventLoopSource* KQueueWaitSet::wait(ClockDurationNano timeout) {
  struct kevent event = {0};

  struct timespec timeoutTS = {
      .tv_sec = timeout.count() / NSEC_PER_SEC,
      .tv_nsec = timeout.count() % NSEC_PER_SEC,
  };

  const struct timespec* ts =
      timeout == ClockDurationNano::max() ? nullptr : &timeoutTS;

  int val = RL_TEMP_FAILURE_RETRY(::kevent(_handle, nullptr, 0, &event, 1, ts));

  RL_ASSERT(val != -1);

  /*
   *  A kevent return value of zero indicates a timeout was encountered.
   */
  return val == 0 ? nullptr : static_cast<EventLoopSource*>(event.udata);
}

WaitSet::Handle KQueueWaitSet::handle() const {
  return _handle;
}

}  // namespace core
}  // namespace rl

#endif  // RL_WAITSET == RL_WAITSET_KQUEUE
