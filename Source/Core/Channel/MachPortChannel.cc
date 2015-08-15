// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <Core/Channel/MachPortChannel.h>
#include <Core/Message.h>
#include <Core/Utilities.h>

#include <mach/mach.h>
#include <sys/event.h>
#include <cassert>

namespace rl {

struct MachPayload {
  mach_msg_header_t header;
  mach_msg_body_t body;
  mach_msg_ool_descriptor_t mem;
  mach_msg_trailer_info_t trailer;

  MachPayload(const Message& message, mach_port_t remote)
      : header({
            .msgh_size = sizeof(MachPayload) - sizeof(mach_msg_trailer_info_t),
            .msgh_remote_port = remote,
            .msgh_bits = MACH_MSGH_BITS_REMOTE(MACH_MSG_TYPE_COPY_SEND) |
                         MACH_MSGH_BITS_COMPLEX,
        }),
        body({.msgh_descriptor_count = 1}),
        mem({
            .address = message.data(),
            .size = static_cast<mach_msg_size_t>(message.size()),
            .deallocate = false,
            .copy = MACH_MSG_VIRTUAL_COPY,
            .type = MACH_MSG_OOL_DESCRIPTOR,
        }),
        trailer() {}

  MachPayload(mach_port_t local)
      : header({.msgh_size = sizeof(MachPayload), .msgh_local_port = local}),
        body(),
        mem(),
        trailer() {}

  bool send() {
    return mach_msg_send(reinterpret_cast<mach_msg_header_t*>(this)) ==
           MACH_MSG_SUCCESS;
  }

  bool receive() {
    return mach_msg_receive(reinterpret_cast<mach_msg_header_t*>(this)) ==
           KERN_SUCCESS;
  }

  Message asMessage() const {
    return Message(static_cast<uint8_t*>(mem.address), mem.size, true);
  }
};

MachPortChannel::MachPortChannel(Channel& channel) : _channel(channel) {
  /*
   *  Step 1: Allocate the port set that will be used as the observer in the
   *          waitset
   */
  kern_return_t res = mach_port_allocate(mach_task_self(),
                                         MACH_PORT_RIGHT_PORT_SET, &_setHandle);
  assert(res == KERN_SUCCESS && _setHandle != MACH_PORT_NULL);

  /*
   *  Step 2: Allocate the port that will be used to send and receive messages
   */
  res = mach_port_allocate(mach_task_self(), MACH_PORT_RIGHT_RECEIVE, &_handle);
  assert(res == KERN_SUCCESS && _handle != MACH_PORT_NULL);

  /*
   *  Step 2: Insert the send port right
   */
  res = mach_port_insert_right(mach_task_self(), _handle, _handle,
                               MACH_MSG_TYPE_MAKE_SEND);
  assert(res == KERN_SUCCESS);

  /*
   *  Step 3: Increase the q limit. The default is 5 which is a bit low. This
   *  may need to be tweaked.
   */

  mach_port_limits_t limits = {0};
  limits.mpl_qlimit = MACH_PORT_QLIMIT_LARGE;
  res = mach_port_set_attributes(
      mach_task_self(), _handle, MACH_PORT_LIMITS_INFO,
      (mach_port_info_t)&limits, MACH_PORT_LIMITS_INFO_COUNT);
  assert(res == KERN_SUCCESS);

  /*
   *  Step 4: Insert the port into the port set. And we are all done.
   */
  res = mach_port_insert_member(mach_task_self(), _handle, _setHandle);
  assert(res == KERN_SUCCESS);
}

MachPortChannel::~MachPortChannel() {
  /*
   *  `doTerminate()` should have already collected the port
   */
}

std::shared_ptr<EventLoopSource> MachPortChannel::createSource() const {
  using ELS = EventLoopSource;

  auto allocator = [&]() { return ELS::Handles(_setHandle, _setHandle); };
  auto readHandler = [&](Handle handle) { _channel.readPendingMessageNow(); };
  auto updateHandler = [](EventLoopSource* source, WaitSet::Handle kev,
                          Handle ident, bool adding) {

    struct kevent event = {0};
    // clang-format off
    EV_SET(&event,                      /* &kev */
           ident,                       /* ident */
           EVFILT_MACHPORT,             /* filter */
           adding ? EV_ADD : EV_DELETE, /* flags */
           0,                           /* fflags */
           0,                           /* data */
           source                       /* udata */);
    // clang-format on

    RL_TEMP_FAILURE_RETRY_AND_CHECK(::kevent(kev, &event, 1, nullptr, 0, NULL));
  };

  return std::make_shared<ELS>(allocator, nullptr, readHandler, nullptr,
                               updateHandler);
}

ChannelProvider::Result MachPortChannel::WriteMessages(
    const Messages& messages) {
  bool success = true;
  for (auto const& message : messages) {
    MachPayload payload(message, _handle);

    if (!payload.send()) {
      success = false;
      break;
    }
  }

  return success ? ChannelProvider::Result::Success
                 : ChannelProvider::Result::PermanentFailure;
}

ChannelProvider::ReadResult MachPortChannel::ReadMessages() {
  MachPayload payload(_handle);
  Messages messages;

  if (payload.receive()) {
    messages.emplace_back(std::move(payload.asMessage()));
  }

  return ChannelProvider::ReadResult(
      messages.size() == 0 ? Result::TemporaryFailure : Result::Success,
      std::move(messages));
}

bool MachPortChannel::doTerminate() {
  kern_return_t res = mach_port_destroy(mach_task_self(), _handle);
  return res == KERN_SUCCESS;
}

}  // namespace rl
