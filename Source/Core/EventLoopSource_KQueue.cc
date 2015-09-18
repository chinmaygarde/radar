// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <Core/Config.h>

#if RL_OS_MAC

#include <Core/Macros.h>
#include <Core/EventLoopSource.h>
#include <Core/WaitSet.h>
#include <Core/Utilities.h>

#include <sys/event.h>
#include <unistd.h>

#define HANDLE_CAST(x) static_cast<int>((x))

namespace rl {

static inline void KEventInvoke(int queue,
                                uintptr_t ident,
                                int16_t filter,
                                uint16_t flags,
                                uint32_t fflags,
                                intptr_t data,
                                void* udata) {
  struct kevent event = {0};
  EV_SET(&event, ident, filter, flags, fflags, data, udata);
  RL_TEMP_FAILURE_RETRY_AND_CHECK(::kevent(queue, &event, 1, nullptr, 0, NULL));
}

void EventLoopSource::updateInWaitSetForSimpleRead(WaitSet& waitset,
                                                   bool shouldAdd) {
  // clang-format off
  KEventInvoke(HANDLE_CAST(waitset.handle()),   /* queue */
               readHandle(),                    /* ident */
               EVFILT_READ,                     /* filter */
               shouldAdd ? EV_ADD : EV_DELETE,  /* flags */
               0,                               /* filter-flags */
               0,                               /* data */
               this);                           /* user-data */
  // clang-format on
}

std::shared_ptr<EventLoopSource> EventLoopSource::Timer(
    std::chrono::nanoseconds repeatInterval) {
  WaitSetUpdateHandler updateHandler =
      [repeatInterval](EventLoopSource& source, WaitSet& waitset,
                       Handle readHandle, bool adding) {
        // clang-format off
        KEventInvoke(HANDLE_CAST(waitset.handle()), /* queue */
                     readHandle,                    /* ident */
                     EVFILT_TIMER,                  /* filter */
                     adding ? EV_ADD : EV_DELETE,   /* flags */
                     NOTE_NSECONDS,                 /* filter-flags */
                     repeatInterval.count(),        /* data */
                     &source);                      /* user-data */
        // clang-format on
      };

  static uintptr_t KQueueTimerIdent = 1;

  RWHandlesProvider handlesProvider =
      [] { return Handles(KQueueTimerIdent++, -1); };

  return std::make_shared<EventLoopSource>(handlesProvider, nullptr, nullptr,
                                           nullptr, updateHandler);
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

#endif  // RL_OS_MAC
