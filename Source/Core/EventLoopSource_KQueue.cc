// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <Core/Config.h>

#if RL_OS_MAC

#include <Core/Macros.h>
#include <Core/EventLoopSource.h>
#include <Core/Utilities.h>

#include <sys/event.h>

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

void EventLoopSource::updateInWaitSetHandleForSimpleRead(
    WaitSet::Handle waitsetHandle,
    bool shouldAdd) {
  // clang-format off
  KEventInvoke(waitsetHandle,                   /* queue */
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
      [repeatInterval](EventLoopSource& source, WaitSet::Handle waitsetHandle,
                       Handle readHandle, bool adding) {
        // clang-format off
        KEventInvoke(waitsetHandle,               /* queue */
                     readHandle,                  /* ident */
                     EVFILT_TIMER,                /* filter */
                     adding ? EV_ADD : EV_DELETE, /* flags */
                     NOTE_NSECONDS,               /* filter-flags */
                     repeatInterval.count(),      /* data */
                     &source);                     /* user-data */
        // clang-format on
      };

  static uintptr_t KQueueTimerIdent = 1;

  RWHandlesProvider handlesProvider =
      [] { return Handles(KQueueTimerIdent++, -1); };

  return std::make_shared<EventLoopSource>(handlesProvider, nullptr, nullptr,
                                           nullptr, updateHandler);
}

}  // namespace rl

#endif  // RL_OS_MAC
