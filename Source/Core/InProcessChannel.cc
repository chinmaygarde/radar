// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <Core/Config.h>

#if RL_CHANNELS == RL_CHANNELS_INPROCESS

#include <Core/InProcessChannel.h>
#include <Core/InProcessChannelAttachment.h>

namespace rl {
namespace core {

InProcessChannel::InProcessChannel(Channel& owner)
    : _attachment(std::make_shared<InProcessChannelAttachment>()),
      _owner(owner) {
  _attachment->addUserspaceCounterpart(owner);
}

InProcessChannel::InProcessChannel(Channel& owner,
                                   const Message::Attachment& attachment)
    : _attachment(std::static_pointer_cast<InProcessChannelAttachment>(
          attachment.handle())),
      _owner(owner) {
  _attachment->addUserspaceCounterpart(owner);
}

InProcessChannel::~InProcessChannel() {
  /*
   *  The removal of the userspace counterpart has already happened in the
   *  `doTerminate` call.
   */
}

std::shared_ptr<EventLoopSource> InProcessChannel::createSource() const {
  return _attachment->createSource();
}

IOResult InProcessChannel::writeMessages(Messages&& messages,
                                         ClockDurationNano timeout) {
  return _attachment->writeMessages(std::move(messages), timeout);
}

IOReadResult InProcessChannel::readMessage(ClockDurationNano timeout) {
  return _attachment->readMessage(timeout);
}

Message::Attachment::Handle InProcessChannel::handle() {
  return _attachment;
}

bool InProcessChannel::doTerminate() {
  _attachment->removeUserspaceCounterpart(_owner);
  return true;
}

}  // namespace core
}  // namespace rl

#endif  // RL_CHANNELS == RL_CHANNELS_INPROCESS
