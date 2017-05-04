/*
 *  This source file is part of the Radar project.
 *  Licensed under the MIT License. See LICENSE file for details.
 */

#include "Config.h"

#if RL_WAITSET == RL_WAITSET_KQUEUE

#include <sys/event.h>
#include <unistd.h>
#include "KQueueWaitSet.h"

#ifndef NSEC_PER_SEC
#define NSEC_PER_SEC 1000000000
#endif

namespace rl {
namespace core {

KQueueWaitSet::KQueueWaitSet() : _handle(RL_TEMP_FAILURE_RETRY(::kqueue())) {
  if (!_handle.isValid()) {
    RL_LOG_ERRNO();
    RL_ASSERT_MSG(false, "Could not create kqueue handle.");
  }
}

KQueueWaitSet::~KQueueWaitSet() = default;

WaitSet::Result KQueueWaitSet::wait(ClockDurationNano timeout) {
  struct kevent event = {};

  struct timespec timeoutTS = {
      .tv_sec = timeout.count() / NSEC_PER_SEC,
      .tv_nsec = timeout.count() % NSEC_PER_SEC,
  };

  const struct timespec* ts =
      timeout == ClockDurationNano::max() ? nullptr : &timeoutTS;

  int val =
      RL_TEMP_FAILURE_RETRY(::kevent(_handle.get(), nullptr, 0, &event, 1, ts));

  RL_ASSERT(val != -1);

  switch (val) {
    case 0:
      /*
       *  A kevent return value of zero indicates a timeout was encountered.
       */
      return WaitSet::Result(WaitSet::WakeReason::Timeout, nullptr);
    case 1:
      /*
       *  The only available kevent source queried woke up.
       */
      return WaitSet::Result(WaitSet::WakeReason::ReadAvailable,
                             static_cast<EventLoopSource*>(event.udata));
    default:
      break;
  }

  return WaitSet::Result(WaitSet::WakeReason::Error, nullptr);
}

WaitSet::Handle KQueueWaitSet::handle() const {
  return _handle.get();
}

}  // namespace core
}  // namespace rl

#endif  // RL_WAITSET == RL_WAITSET_KQUEUE
