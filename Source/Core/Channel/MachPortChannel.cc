// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "Core/Channel/MachPortChannel.h"
#include "Core/Message.h"
#include "Core/Utilities.h"

#include <mach/mach.h>
#include <sys/event.h>
#include <cassert>

namespace rl {

MachPortChannel::MachPortChannel(const std::string& name)
    : Channel(name), _source() {
  /*
   *  Step 1: Allocate the port
   */
  kern_return_t res =
      mach_port_allocate(mach_task_self(), MACH_PORT_RIGHT_PORT_SET, &_handle);
  assert(res == KERN_SUCCESS && _handle != MACH_PORT_NULL);

#if 0
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
#endif
}

MachPortChannel::~MachPortChannel() {
}

Channel::ConnectedPair MachPortChannel::CreateConnectedPair() {
  auto port = std::make_shared<MachPortChannel>("rl.connected");
  return ConnectedPair(port, port);
}

std::shared_ptr<LooperSource> MachPortChannel::source() {
  using LS = LooperSource;

  auto allocator = [&]() { return LS::Handles(_handle, _handle); };
  auto readHandler = [](Handle handle) { assert(false); };

  auto source = std::make_shared<LS>(allocator, nullptr, readHandler, nullptr);

  auto updateHandler =
      [source](LooperSource* source, WaitSet::Handle waitsetHandle,
               Handle readHandle, bool adding) {

        struct kevent event = {0};
        // clang-format off
        EV_SET(&event,                      /* &kev */
               readHandle,                  /* ident */
               EVFILT_MACHPORT,             /* filter */
               adding ? EV_ADD : EV_DELETE, /* flags */
               MACH_RCV_MSG,                /* fflags */
               0,                           /* data */
               source                       /* udata */);
        // clang-format on

        RL_TEMP_FAILURE_RETRY_AND_CHECK(
            ::kevent(waitsetHandle, &event, 1, nullptr, 0, NULL));
      };

  source->setCustomWaitSetUpdateHandler(updateHandler);

  return source;
}

Channel::Result MachPortChannel::WriteMessage(Message& message) {
  /*
   *  Since there is no scatter/gather equivalent to the mach_msg API, we depend
   *  on the caller to allocate the appropriate framing header for us. This
   *  avoids a stupid copy but adds some complexity to the API :/
   */
  assert(message.reservedHeaderSize() == sizeof(mach_msg_header_t));

  mach_msg_header_t* msg = reinterpret_cast<mach_msg_header_t*>(message.data());
  msg->msgh_size =
      sizeof(mach_msg_header_t) + static_cast<mach_msg_size_t>(message.size());
  msg->msgh_remote_port = _handle;
  msg->msgh_bits = MACH_MSGH_BITS(MACH_MSG_TYPE_COPY_SEND, 0);

  kern_return_t res =
      mach_msg(msg, MACH_SEND_MSG, msg->msgh_size, 0, MACH_PORT_NULL,
               MACH_MSG_TIMEOUT_NONE, MACH_PORT_NULL);

  assert(res == MACH_MSG_SUCCESS);

  return res == MACH_MSG_SUCCESS ? Channel::Result::Success
                                 : Channel::Result::PermanentFailure;
}

Channel::ReadResult MachPortChannel::ReadMessages() {
  return Channel::ReadResult(Result::PermanentFailure,
                             std::vector<std::unique_ptr<Message>>());
}

bool MachPortChannel::doConnect(const std::string& endpoint) {
  assert(false && "Unimplemented");
  return false;
}

bool MachPortChannel::doTerminate() {
  kern_return_t res = mach_port_destroy(mach_task_self(), _handle);
  return res == KERN_SUCCESS;
}

Message MachPortChannel::createMessage(size_t reserved) const {
  return Message(sizeof(mach_msg_header_t), reserved);
}

}  // namespace rl
