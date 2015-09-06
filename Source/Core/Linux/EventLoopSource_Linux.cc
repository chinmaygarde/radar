// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <Core/Macros.h>
#include <Core/EventLoopSource.h>
#include <Core/Utilities.h>

#include <sys/epoll.h>
#include <sys/timerfd.h>
#include <unistd.h>

#ifndef NSEC_PER_SEC
#define NSEC_PER_SEC 1000000000 /* nanoseconds per second */
#endif

namespace rl {

static inline void EPollInvoke(int eventsMask,
                               void* data,
                               int epollDesc,
                               int operation,
                               int desc) {
  struct epoll_event event = {0};

  event.events = eventsMask;
  event.data.ptr = data; /* union */

  RL_TEMP_FAILURE_RETRY(::epoll_ctl(epollDesc, operation, desc, &event));
}

void EventLoopSource::updateInWaitSetHandleForSimpleRead(
    WaitSet::Handle waitsetHandle,
    bool shouldAdd) {
  EPollInvoke(EPOLLIN, this, waitsetHandle,
              shouldAdd ? EPOLL_CTL_ADD : EPOLL_CTL_DEL, readHandle());
}

std::shared_ptr<EventLoopSource> EventLoopSource::Timer(
    std::chrono::nanoseconds repeatInterval) {
  RWHandlesProvider provider = [repeatInterval]() {
    /*
     *  Create and arm the timer file descriptor
     */
    Handle desc = timerfd_create(CLOCK_MONOTONIC, 0);

    const uint64_t nano_secs = repeatInterval.count();

    struct itimerspec spec = {0};

    spec.it_value.tv_sec = (time_t)(nano_secs / NSEC_PER_SEC);
    spec.it_value.tv_nsec = nano_secs % NSEC_PER_SEC;

    spec.it_interval = spec.it_value;

    RL_CHECK(::timerfd_settime(desc, 0, &spec, nullptr));

    return Handles(desc, -1);
  };

  RWHandlesCollector collector = [](Handles handles) {
    RL_ASSERT(handles.second == -1 /* since we never assigned one */);
    RL_CHECK(::close(handles.first));
  };

  IOHandler reader = [](Handle r) {
    /*
     *  8 bytes must be read from a signalled timer file descriptor when
     *  signalled.
     */
    uint64_t fireCount = 0;

    ssize_t size =
        RL_TEMP_FAILURE_RETRY(::read(r, &fireCount, sizeof(uint64_t)));

    RL_ASSERT(size == sizeof(uint64_t));
  };

  return std::make_shared<EventLoopSource>(provider, collector, reader, nullptr,
                                           nullptr);
}

}  // namespace rl
