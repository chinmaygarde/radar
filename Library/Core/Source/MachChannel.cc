/*
 *  This source file is part of the Radar project.
 *  Licensed under the MIT License. See LICENSE file for details.
 */

#include "Config.h"

#if RL_CHANNELS == RL_CHANNELS_MACH

#include <Core/Message.h>
#include <Core/Utilities.h>
#include <sys/event.h>
#include "MachChannel.h"

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
      _port(std::make_shared<MachPort>(std::move(attachment))) {
  auto success = setupPortSetMemberships();
  RL_ASSERT(success);
}

bool MachChannel::setupPortSetMemberships() {
  if (_port == nullptr || !_port->isValid()) {
    return false;
  }

  _set = std::make_unique<MachPort>(MachPort::Type::PortSet);

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

  auto setHandle = _set->name();
  auto allocator = [setHandle]() {
    return EventLoopSource::Handles(setHandle, setHandle);
  };
  auto readHandler = [&](EventLoopSource::Handle) {
    return _channel.readPendingMessageNow();
  };

  auto updateHandler = [](EventLoopSource& source, WaitSet& kev,
                          EventLoopSource::Handle ident, bool adding) {
    struct kevent event = {};

    EV_SET(&event,                      /* &kev */
           ident,                       /* ident */
           EVFILT_MACHPORT,             /* filter */
           adding ? EV_ADD : EV_DELETE, /* flags */
           0,                           /* fflags */
           0,                           /* data */
           &source /* udata */);

    RL_TEMP_FAILURE_RETRY_AND_CHECK(
        ::kevent(static_cast<int>(kev.handle()), &event, 1, nullptr, 0, NULL));
  };

  return std::make_shared<EventLoopSource>(
      allocator,    /* handleProvider */
      nullptr,      /* handleCollector */
      readHandler,  /* readHandler */
      nullptr,      /* writeHandler */
      updateHandler /* waitsetUpdateHandler */
      );
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
