// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "MachTrivialSource.h"

#include <sys/event.h>
#include <mach/mach.h>

namespace rl {

/*
 *  Theory of Operation:
 *  We want a trivial source that can be signalled multiple times but on which
 *  only one sucessful read is allowed. Think of this as analogous to event FDs
 *  on Linux. To simulate this behavior, we setup a mach port with a q limit of
 *  1 and make all sends (and receives) take a zero timeout. This way, multiple
 *  sends on a signalled port time out.
 *
 *  We cannot use pipes since that will require multiple syscalls to drain all
 *  writes.
 */

MachTrivialSource::MachTrivialSource()
    : EventLoopSource(), _port(1 /* q_limit*/) {
  setHandlesProvider([&]() {
    /*
     *  Vend the portset handles
     */
    return Handles(_port.setHandle(), _port.setHandle());
  });

  setWriter([&](Handle handle) {
    Messages messages;
    messages.emplace_back();
    auto result =
        _port.sendMessages(std::move(messages), ClockDurationMilli(0));
    RL_ASSERT(result != MachPort::Result::Failure);

    /*
     *  Timeouts are fine, and expected if the port is already signalled
     *  but the send rejected due to the q limit being hit.
     */
  });

  setReader([&](Handle handle) {
    auto result = _port.readMessages(ClockDurationMilli(0));
    RL_ASSERT(result.first != MachPort::Result::Failure);
    /*
     *  We don't actually care about the message contents. Just need to
     *  ensure that a round trip was made
     */
  });

  /*
   *  We are using a mach port instead of regular file desriptor for reading.
   *  This requires a custom waitset handler.
   */
  setCustomWaitSetUpdateHandler([](EventLoopSource& source, WaitSet& kev,
                                   EventLoopSource::Handle ident, bool adding) {

    // clang-format off
    struct kevent event = {0};

    EV_SET(&event,                      /* &kev */
           ident,                       /* ident */
           EVFILT_MACHPORT,             /* filter */
           adding ? EV_ADD : EV_DELETE, /* flags */
           0,                           /* fflags */
           0,                           /* data */
           &source                      /* udata */);

    RL_TEMP_FAILURE_RETRY_AND_CHECK(::kevent(static_cast<int>(kev.handle()),
                                             &event,
                                             1,
                                             nullptr,
                                             0,
                                             NULL));
    // clang-format on
  });
}

MachTrivialSource::~MachTrivialSource() {
  auto res = _port.doTerminate();
  RL_ASSERT(res);
}

}  // namespace rl
