// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <Core/Macros.h>
#include <Core/LooperSource.h>
#include <Core/Utilities.h>

#include <sys/event.h>

namespace rl {

static inline void LooperSource_UpdateKeventSource(int queue,
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

void LooperSource::updateInWaitSetHandle(WaitSet::Handle waitsetHandle,
                                         bool shouldAdd) {
  if (_customWaitSetUpdateHandler) {
    _customWaitSetUpdateHandler(this, waitsetHandle, readHandle(), shouldAdd);
    return;
  }

  LooperSource_UpdateKeventSource(waitsetHandle, readHandle(), EVFILT_READ,
                                  shouldAdd ? EV_ADD : EV_DELETE, 0, 0, this);
}

std::shared_ptr<LooperSource> LooperSource::AsTimer(
    std::chrono::nanoseconds repeatInterval) {
  WaitSetUpdateHandler updateHandler = [repeatInterval](
      LooperSource* source, WaitSet::Handle waitsetHandle, Handle readHandle,
      bool adding) {

    LooperSource_UpdateKeventSource(waitsetHandle, readHandle, EVFILT_TIMER,
                                    adding ? EV_ADD : EV_DELETE, NOTE_NSECONDS,
                                    repeatInterval.count(), source);

  };

  static uintptr_t KQueueTimerIdent = 1;

  RWHandlesProvider handlesProvider =
      [] { return Handles(KQueueTimerIdent++, -1); };

  auto timer = std::make_shared<LooperSource>(handlesProvider, nullptr, nullptr,
                                              nullptr);

  timer->setCustomWaitSetUpdateHandler(updateHandler);

  return timer;
}

}  // namespace rl
