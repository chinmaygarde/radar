// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <Core/Config.h>

#if RL_CHANNELS == RL_CHANNELS_MACH

#include "MachTrivialSource.h"

#include <mach/mach.h>
#include <sys/event.h>

namespace rl {
namespace core {

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
    EventLoopSource::Handle handle = _port.setAttachment().handle();
    return Handles(handle, handle);
  });

  setWriter([&](Handle) {
    Messages messages;
    messages.emplace_back();
    auto result =
        _port.sendMessages(std::move(messages), ClockDurationMilli(0));
    RL_ASSERT(result != IOResult::Failure);

    /*
     *  Timeouts are fine, and expected if the port is already signalled
     *  but the send rejected due to the q limit being hit.
     */
    return result;
  });

  setReader([&](Handle) {
    auto result = _port.readMessage(ClockDurationMilli(0));
    RL_ASSERT(result.first != IOResult::Failure);
    /*
     *  We don't actually care about the message contents. Just need to
     *  ensure that a round trip was made
     */
    return result.first;
  });

  /*
   *  We are using a mach port instead of regular file desriptor for reading.
   *  This requires a custom waitset handler.
   */
  setCustomWaitSetUpdateHandler([](EventLoopSource& source, WaitSet& kev,
                                   EventLoopSource::Handle ident, bool adding) {
    // clang-format off
    struct kevent event = {};

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

}  // namespace core
}  // namespace rl

#endif  //  RL_CHANNELS == RL_CHANNELS_MACH
