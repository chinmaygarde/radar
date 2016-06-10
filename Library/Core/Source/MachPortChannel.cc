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
    : _channel(channel),
      _port(std::make_shared<MachPort>(MachPort::Type::SendReceive)) {
  setupPortSetMemberships();
}

MachPortChannel::MachPortChannel(Channel& channel, RawAttachment attachment)
    : _channel(channel),
      _port(std::make_shared<MachPort>(MachPort::Type::Send,
                                       attachment.takeHandle())) {
  setupPortSetMemberships();
}

bool MachPortChannel::setupPortSetMemberships() {
  if (_port == nullptr || !_port->isValid()) {
    return false;
  }

  if (_port->type() == MachPort::Type::SendReceive) {
    /*
     *  We only need to create the port set for this channel if the primary port
     *  is capable of receiving messages, i.e, has a receive right.
     */
    _set = core::make_unique<MachPort>(MachPort::Type::PortSet);

    if (!_set->isValid()) {
      /*
       *  Check portset allocation validity.
       */
      return false;
    }

    /*
     *  Attempt to insert the port into the newly setup portset.
     */
    return _set->insertMember(*_port);
  }

  /*
   *  We are done.
   */
  return true;
}

bool MachPortChannel::teardownPortSetMemberships() {
  if (_port == nullptr || !_port->isValid()) {
    return true;
  }

  if (_port->type() == MachPort::Type::SendReceive) {
    /*
     *  If the primary port is a receive type port, we had added the same to the
     *  portset. We must not collect the same.
     */

    if (_set == nullptr || !_set->isValid()) {
      return false;
    }

    return _set->extractMember(*_port);
  }

  /*
   *  We are done.
   */
  return true;
}

MachPortChannel::~MachPortChannel() {
  /*
   *  `doTerminate()` should have already collected the port
   */
}

std::shared_ptr<EventLoopSource> MachPortChannel::createSource() const {
  using ELS = EventLoopSource;

  auto setHandle = _port->name();
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
  return _port->sendMessages(std::move(messages), timeout);
}

IOReadResult MachPortChannel::readMessage(ClockDurationNano timeout) {
  return _port->receiveMessage(timeout);
}

AttachmentRef MachPortChannel::attachment() {
  return _port;
}

bool MachPortChannel::doTerminate() {
  return teardownPortSetMemberships();
}

}  // namespace core
}  // namespace rl

#endif  // RL_CHANNELS == RL_CHANNELS_MACH
