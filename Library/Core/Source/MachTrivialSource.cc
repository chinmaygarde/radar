/*
 *  This source file is part of the Radar project.
 *  Licensed under the MIT License. See LICENSE file for details.
 */

#include "Config.h"

#if RL_CHANNELS == RL_CHANNELS_MACH

#include <Core/WaitSet.h>
#include <mach/mach.h>
#include <sys/event.h>
#include "MachTrivialSource.h"

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
    : EventLoopSource(),
      _port(MachPort::Type::SendReceive),
      _set(MachPort::Type::PortSet) {
  auto res = _port.setQueueLimit(1);
  RL_ASSERT(res);

  res = _set.insertMember(_port);
  RL_ASSERT(res);

  setHandlesProvider([&]() {
    /*
     *  Vend the portset handles
     */
    EventLoopSource::Handle handle = _set.name();
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
    auto result = _port.receiveMessage(ClockDurationMilli(0));
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
    struct kevent event = {};

    EV_SET(&event,                      /* &kev */
           ident,                       /* ident */
           EVFILT_MACHPORT,             /* filter */
           adding ? EV_ADD : EV_DELETE, /* flags */
           0,                           /* fflags */
           0,                           /* data */
           &source                      /* udata */
           );

    RL_TEMP_FAILURE_RETRY_AND_CHECK(::kevent(
        static_cast<int>(kev.handle()), /* kq */
        &event,                         /* changelist */
        1,                              /* nchanges */
        nullptr,                        /* eventlist */
        0,                              /* nevents */
        NULL                            /* timeout */
        ));
  });
}

MachTrivialSource::~MachTrivialSource() {
  auto res = _set.extractMember(_port);
  RL_ASSERT(res);
}

}  // namespace core
}  // namespace rl

#endif  //  RL_CHANNELS == RL_CHANNELS_MACH
