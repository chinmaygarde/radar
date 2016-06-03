// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <Core/Config.h>

#if RL_CHANNELS == RL_CHANNELS_MACH

#include "MachPortChannel.h"

#include <Core/Message.h>
#include <Core/Utilities.h>

#include <sys/event.h>

namespace rl {
namespace core {

MachPortChannel::MachPortChannel(Channel& channel)
    : _channel(channel), _port(1024) {}

MachPortChannel::MachPortChannel(Channel& channel, const Attachment& attachment)
    : _channel(channel), _port(attachment) {}

MachPortChannel::~MachPortChannel() {
  /*
   *  `doTerminate()` should have already collected the port
   */
}

std::shared_ptr<EventLoopSource> MachPortChannel::createSource() const {
  using ELS = EventLoopSource;

  auto setHandle = _port.setAttachment().handle();
  auto allocator = [setHandle]() { return ELS::Handles(setHandle, setHandle); };
  auto readHandler = [&](ELS::Handle) {
    return _channel.readPendingMessageNow();
  };

  // clang-format off
  auto updateHandler = [](EventLoopSource& source,
                          WaitSet& kev,
                          ELS::Handle ident,
                          bool adding) {
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
  };

  return std::make_shared<ELS>(allocator,
                               nullptr,
                               readHandler,
                               nullptr,
                               updateHandler);
  // clang-format on
}

IOResult MachPortChannel::writeMessages(Messages&& messages,
                                        ClockDurationNano timeout) {
  return _port.sendMessages(std::move(messages), timeout);
}

IOReadResult MachPortChannel::readMessage(ClockDurationNano timeout) {
  return _port.readMessage(timeout);
}

const Attachment& MachPortChannel::attachment() {
  return _port.portAttachment();
}

bool MachPortChannel::doTerminate() {
  return _port.doTerminate();
}

}  // namespace core
}  // namespace rl

#endif  // RL_CHANNELS == RL_CHANNELS_MACH
