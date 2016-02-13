// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <Core/Config.h>

#if RL_CHANNELS == RL_CHANNELS_INPROCESS

#include <Core/InProcessChannel.h>
#include <Core/InProcessChannelActual.h>

namespace rl {
namespace core {

InProcessChannel::InProcessChannel(Channel& owner)
    : _actual(std::make_shared<InProcessChannelActual>()), _owner(owner) {
  _actual->addUserspaceCounterpart(owner);
}

InProcessChannel::InProcessChannel(Channel& owner,
                                   const Message::Attachment& attachment)
    : _owner(owner) {
  _actual = *reinterpret_cast<std::shared_ptr<InProcessChannelActual>*>(
      attachment.handle());
  _actual->addUserspaceCounterpart(owner);
}

InProcessChannel::~InProcessChannel() {
  /*
   *  The removal of the userspace counterpart has already happened in the
   *  `doTerminate` call.
   */
}

std::shared_ptr<EventLoopSource> InProcessChannel::createSource() const {
  return _actual->createSource();
}

IOResult InProcessChannel::writeMessages(Messages&& messages,
                                         ClockDurationNano timeout) {
  return _actual->writeMessages(std::move(messages), timeout);
}

IOReadResult InProcessChannel::readMessage(ClockDurationNano timeout) {
  return _actual->readMessage(timeout);
}

Message::Attachment::Handle InProcessChannel::handle() {
  return reinterpret_cast<Message::Attachment::Handle>(&_actual);
}

bool InProcessChannel::doTerminate() {
  _actual->removeUserspaceCounterpart(_owner);
  return true;
}

}  // namespace core
}  // namespace rl

#endif  // RL_CHANNELS == RL_CHANNELS_INPROCESS
