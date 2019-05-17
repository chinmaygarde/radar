/*
 *  This source file is part of the Radar project.
 *  Licensed under the MIT License. See LICENSE file for details.
 */

#include "Config.h"

#if RL_WAITSET == RL_WAITSET_KQUEUE

#include <Core/EventLoopSource.h>
#include <Core/Macros.h>
#include <Core/Utilities.h>
#include <Core/WaitSet.h>
#include <sys/event.h>
#include <unistd.h>
#include "MachTrivialSource.h"

#define HANDLE_CAST(x) static_cast<int>((x))

namespace rl {
namespace core {

static inline void KEventInvoke(int queue,
                                uintptr_t ident,
                                int16_t filter,
                                uint16_t flags,
                                uint32_t fflags,
                                intptr_t data,
                                void* udata) {
  struct kevent event = {};
  EV_SET(&event, ident, filter, flags, fflags, data, udata);
  RL_TEMP_FAILURE_RETRY(::kevent(queue, &event, 1, nullptr, 0, NULL));
}

void EventLoopSource::updateInWaitSetForSimpleRead(WaitSet& waitset,
                                                   bool shouldAdd) {
  KEventInvoke(HANDLE_CAST(waitset.handle()),  /* queue */
               handles().readHandle,           /* ident */
               EVFILT_READ,                    /* filter */
               shouldAdd ? EV_ADD : EV_DELETE, /* flags */
               0,                              /* filter-flags */
               0,                              /* data */
               this);                          /* user-data */
}

std::shared_ptr<EventLoopSource> EventLoopSource::Timer(
    ClockDurationNano repeatInterval) {
  WaitSetUpdateHandler updateHandler = [repeatInterval](
                                           EventLoopSource& source,  //
                                           WaitSet& waitset,         //
                                           Handle readHandle,        //
                                           bool adding) {
    KEventInvoke(HANDLE_CAST(waitset.handle()), /* queue */
                 readHandle,                    /* ident */
                 EVFILT_TIMER,                  /* filter */
                 adding ? EV_ADD : EV_DELETE,   /* flags */
                 NOTE_NSECONDS,                 /* filter-flags */
                 repeatInterval.count(),        /* data */
                 &source);                      /* user-data */
  };

  static uintptr_t KQueueTimerIdent = 1;

  RWHandlesProvider handlesProvider = [] {
    return Handles(KQueueTimerIdent++, -1);
  };

  return std::make_shared<EventLoopSource>(handlesProvider, nullptr, nullptr,
                                           nullptr, updateHandler);
}

std::shared_ptr<EventLoopSource> EventLoopSource::Trivial() {
/*
 *  Mach sources are preferred over dumb pipes but the BSDs also use kqueue
 *  but dont have Mach. Fallback to pipes on the same.
 */
#if RL_CHANNELS == RL_CHANNELS_MACH
  return std::make_shared<MachTrivialSource>();
#else
  return MakePipeBasedTrivialSource();
#endif
}

}  // namespace core
}  // namespace rl

#endif  // RL_WAITSET == RL_WAITSET_KQUEUE
