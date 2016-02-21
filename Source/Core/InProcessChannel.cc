// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <Core/Config.h>

#if RL_CHANNELS == RL_CHANNELS_INPROCESS

#include <Core/InProcessChannel.h>
#include <Core/InProcessChannelAttachment.h>
#include <Core/InProcessWaitSet.h>
#include <Core/Channel.h>

namespace rl {
namespace core {

InProcessChannel::InProcessChannel(Channel& owner)
    : _attachment(std::make_shared<InProcessChannelAttachment>()),
      _owner(owner) {
  _attachment->registerUserspaceChannel(_owner);
}

InProcessChannel::InProcessChannel(Channel& owner,
                                   const Message::Attachment& attachment)
    : _attachment(std::static_pointer_cast<InProcessChannelAttachment>(
          attachment.handle())),
      _owner(owner) {
  RL_ASSERT_MSG(
      _attachment,
      "Must be able to materialize a valid channel from an attachment");
  _attachment->registerUserspaceChannel(_owner);
}

InProcessChannel::~InProcessChannel() {
  /*
   *  The removal of the userspace counterpart has already happened in the
   *  `doTerminate` call.
   */
}

std::shared_ptr<EventLoopSource> InProcessChannel::createSource() const {
  auto handle = reinterpret_cast<EventLoopSource::Handle>(this);

  EventLoopSource::RWHandlesProvider allocator = [handle]() {
    /*
     *  Since this channel is going to write to this source as well are read
     *  from it, we assign ourselves as the read and write handles.
     */
    return EventLoopSource::Handles(handle, handle);
  };

  EventLoopSource::IOHandler readHandler = [&](EventLoopSource::Handle) {
    return _owner.readPendingMessageNow();
  };

  EventLoopSource::WaitSetUpdateHandler updateHandler = [&](
      EventLoopSource&, WaitSet& waitset, EventLoopSource::Handle,
      bool adding) {
    auto& inprocessWaitset = reinterpret_cast<InProcessWaitSet&>(waitset);
    if (adding) {
      _attachment->addSubscriberWaitset(inprocessWaitset);
    } else {
      _attachment->removeSubscriberWaitset(inprocessWaitset);
    }
  };

  return std::make_shared<EventLoopSource>(allocator, nullptr, readHandler,
                                           nullptr, updateHandler);
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
  _attachment->unregisterUserspaceChannel(_owner);
  return true;
}

}  // namespace core
}  // namespace rl

#endif  // RL_CHANNELS == RL_CHANNELS_INPROCESS
