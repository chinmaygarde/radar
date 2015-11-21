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
#include <sys/eventfd.h>
#include <unistd.h>

#define HANDLE_CAST(x) static_cast<int>((x))

#ifndef NSEC_PER_SEC
#define NSEC_PER_SEC 1000000000 /* nanoseconds per second */
#endif

namespace rl {
namespace core {

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
  RWHandlesProvider provider = [] {
    int efd = eventfd(0 /* initial value */, 0 /* flags */);
    return Handles(efd, efd);
  };

  RWHandlesCollector collector = [](Handles h) {
    RL_ASSERT(h.first == h.second);
    RL_CHECK(::close(HANDLE_CAST(h.first)));
  };

  IOHandler reader = [](Handle r) {
    /*
     *  On read, the signal count is the number of times this descriptor was
     *  woken up.
     */
    uint64_t signalCount = 0;

    ssize_t size = RL_TEMP_FAILURE_RETRY(
        ::read(HANDLE_CAST(r), &signalCount, sizeof(signalCount)));

    RL_ASSERT(signalCount > 0);
  };

  IOHandler writer = [](Handle w) {
    /*
     *  On signal, the 8 byte integer value is addded. We specify 1 for each
     *  wake
     */
    const uint64_t writeCount = 1;

    ssize_t size = RL_TEMP_FAILURE_RETRY(
        ::write(HANDLE_CAST(w), &writeCount, sizeof(writeCount)));

    RL_ASSERT(size == sizeof(writeCount));
  };

  return std::make_shared<EventLoopSource>(provider, collector, reader, writer,
                                           nullptr);
}

}  // namespace core
}  // namespace rl

#endif  // RL_WAITSET == RL_WAITSET_EPOLL