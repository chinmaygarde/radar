// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <Core/Config.h>

#if !RL_OS_MAC && !RL_OS_LINUX

#include <Core/WaitSet.h>
#include <Core/EventLoopSource.h>
#include <Core/Utilities.h>

#include <sys/poll.h>
#include <unistd.h>

namespace rl {

WaitSet::Handle WaitSet::platformHandleCreate() {
  /*
   *  Poll support requires no platform handle
   */
  return -1;
}

EventLoopSource& WaitSet::platformHandleWait(WaitSet::Handle handle) {
  nfds_t count = static_cast<nfds_t>(_sources.size());

  struct pollfd pollfds[count];

  auto index = 0;
  for (const auto& source : _sources) {
    pollfds[index].fd = source->readHandle();
    pollfds[index].events = POLLIN;
    pollfds[index].revents = 0;
    index++;
  }

  while (true) {
    auto result = poll(pollfds, count, -1 /* timeout */);
    if (result == 0 || (result == -1 && errno == EINTR)) {
      /*
       *  A timeout expired or the task received an EINTR. Re-arm the timers
       *  on next run
       */
      // TODO: Rearm timers
    } else if (result > 0) {
      /*
       *  At least one source has data available for reading. Return the first
       *  result
       */
      for (auto i = 0; i < count; i++) {
        if (pollfds[i].revents & POLLIN) {
          return *_sources[i];
        }
      }
      RL_ASSERT(false &&
                "Poll must not return a result > 0 without POLLIN on any "
                "descriptors in the resulting set");
    } else {
      RL_ASSERT(false);
    }
  }

  RL_ASSERT(false);
}

void WaitSet::platformHandleDestory(WaitSet::Handle handle) {
  /*
   *  No handle to collect
   */
}

}  // namespace rl

#endif  // !RL_OS_MAC && !RL_OS_LINUX
