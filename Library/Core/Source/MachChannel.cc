// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <Core/Config.h>

#if RL_CHANNELS == RL_CHANNELS_MACH

#include "MachChannel.h"

#include <Core/Message.h>
#include <Core/Utilities.h>

#include <sys/event.h>

namespace rl {
namespace core {

MachChannel::MachChannel(Channel& channel)
    : _channel(channel),
      _port(std::make_shared<MachPort>(MachPort::Type::SendReceive)) {
  auto success = setupPortSetMemberships();
  RL_ASSERT(success);
}

MachChannel::MachChannel(Channel& channel, RawAttachment attachment)
    : _channel(channel),
      _port(std::make_shared<MachPort>(MachPort::Type::Send,
                                       attachment.takeAttachmentHandle())) {
  auto success = setupPortSetMemberships();
  RL_ASSERT(success);
}

bool MachChannel::setupPortSetMemberships() {
  if (_port == nullptr || !_port->isValid()) {
    return false;
  }

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

bool MachChannel::teardownPortSetMemberships() {
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

MachChannel::~MachChannel() {
  /*
   *  `doTerminate()` should have already collected the port
   */
}

std::shared_ptr<EventLoopSource> MachChannel::createSource() const {
  if (_set == nullptr || !_set->isValid()) {
    return nullptr;
  }

  using ELS = EventLoopSource;

  auto setHandle = _set->name();
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

IOResult MachChannel::writeMessages(Messages&& messages,
                                    ClockDurationNano timeout) {
  return _port->sendMessages(std::move(messages), timeout);
}

IOReadResult MachChannel::readMessage(ClockDurationNano timeout) {
  return _port->receiveMessage(timeout);
}

AttachmentRef MachChannel::attachment() {
  return _port;
}

bool MachChannel::doTerminate() {
  return teardownPortSetMemberships();
}

}  // namespace core
}  // namespace rl

#endif  // RL_CHANNELS == RL_CHANNELS_MACH