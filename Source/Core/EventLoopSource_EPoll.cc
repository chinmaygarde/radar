// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <Core/Config.h>

#if RL_WAITSET == RL_WAITSET_EPOLL

#include <Core/Macros.h>
#include <Core/EventLoopSource.h>
#include <Core/WaitSet.h>
#include <Core/Utilities.h>

#include <sys/epoll.h>
#include <sys/timerfd.h>
#include <unistd.h>

#define HANDLE_CAST(x) static_cast<int>((x))

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

void EventLoopSource::updateInWaitSetForSimpleRead(WaitSet& waitset,
                                                   bool shouldAdd) {
  // clang-format off
  EPollInvoke(EPOLLIN,
              this,
              HANDLE_CAST(waitset.handle()),
              shouldAdd ? EPOLL_CTL_ADD : EPOLL_CTL_DEL,
              readHandle());
  // clang-format on
}

std::shared_ptr<EventLoopSource> EventLoopSource::Timer(
    ClockDurationNano repeatInterval) {
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

std::shared_ptr<EventLoopSource> EventLoopSource::Trivial() {
  /*
   *  We are using a simple pipe but this should likely be something
   *  that coalesces multiple writes. Something like an event_fd on Linux
   */
  RWHandlesProvider provider = [] {
    int descriptors[2] = {0};

    RL_CHECK(::pipe(descriptors));

    return Handles(descriptors[0], descriptors[1]);
  };

  RWHandlesCollector collector = [](Handles h) {
    RL_CHECK(::close(HANDLE_CAST(h.first)));
    RL_CHECK(::close(HANDLE_CAST(h.second)));
  };

  static const char EventLoopWakeMessage[] = "w";

  IOHandler reader = [](Handle r) {
    char buffer[sizeof(EventLoopWakeMessage) / sizeof(char)];

    ssize_t size =
        RL_TEMP_FAILURE_RETRY(::read(HANDLE_CAST(r), &buffer, sizeof(buffer)));

    RL_ASSERT(size == sizeof(buffer));
  };

  IOHandler writer = [](Handle w) {

    ssize_t size = RL_TEMP_FAILURE_RETRY(::write(
        HANDLE_CAST(w), EventLoopWakeMessage, sizeof(EventLoopWakeMessage)));

    RL_ASSERT(size == sizeof(EventLoopWakeMessage));
  };

  return std::make_shared<EventLoopSource>(provider, collector, reader, writer,
                                           nullptr);
}

}  // namespace rl

#endif
