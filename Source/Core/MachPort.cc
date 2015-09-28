// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "MachPort.h"

#include <mach/mach.h>

namespace rl {

struct MachPayload {
  mach_msg_header_t header;
  mach_msg_body_t body;
  mach_msg_ool_descriptor_t mem;
  mach_msg_trailer_info_t trailer;

  explicit MachPayload(const Message& message, mach_port_t remote)
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

  explicit MachPayload(mach_port_t local)
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

MachPort::MachPort() {
  /*
   *  Step 1: Allocate the port set that will be used as the observer in the
   *          waitset
   */
  kern_return_t res = mach_port_allocate(mach_task_self(),
                                         MACH_PORT_RIGHT_PORT_SET, &_setHandle);
  RL_ASSERT(res == KERN_SUCCESS && _setHandle != MACH_PORT_NULL);

  /*
   *  Step 2: Allocate the port that will be used to send and receive messages
   */
  res = mach_port_allocate(mach_task_self(), MACH_PORT_RIGHT_RECEIVE, &_handle);
  RL_ASSERT(res == KERN_SUCCESS && _handle != MACH_PORT_NULL);

  /*
   *  Step 2: Insert the send port right
   */
  res = mach_port_insert_right(mach_task_self(), _handle, _handle,
                               MACH_MSG_TYPE_MAKE_SEND);
  RL_ASSERT(res == KERN_SUCCESS);

  /*
   *  Step 3: Increase the q limit. The default is 5 which is a bit low. This
   *  may need to be tweaked.
   */

  mach_port_limits_t limits = {0};
  limits.mpl_qlimit = MACH_PORT_QLIMIT_LARGE;
  res = mach_port_set_attributes(
      mach_task_self(), _handle, MACH_PORT_LIMITS_INFO,
      (mach_port_info_t)&limits, MACH_PORT_LIMITS_INFO_COUNT);
  RL_ASSERT(res == KERN_SUCCESS);

  /*
   *  Step 4: Insert the port into the port set. And we are all done.
   */
  res = mach_port_insert_member(mach_task_self(), _handle, _setHandle);
  RL_ASSERT(res == KERN_SUCCESS);
}

MachPort::~MachPort() {
  /*
   *  `doTerminate()` should have already collected the port
   */
  RL_ASSERT(_handle == MACH_PORT_NULL && _setHandle == MACH_PORT_NULL);
}

MachPort::Handle MachPort::portHandle() const {
  return _handle;
}

MachPort::Handle MachPort::setHandle() const {
  return _setHandle;
}

bool MachPort::doTerminate() {
  kern_return_t res0 =
      mach_port_extract_member(mach_task_self(), _handle, _setHandle);
  kern_return_t res1 = mach_port_destroy(mach_task_self(), _handle);
  kern_return_t res2 = mach_port_destroy(mach_task_self(), _setHandle);

  auto success =
      res0 == KERN_SUCCESS && res1 == KERN_SUCCESS && res2 == KERN_SUCCESS;

  if (success) {
    _handle = MACH_PORT_NULL;
    _setHandle = MACH_PORT_NULL;
  }

  return success;
}

MachPort::Result MachPort::sendMessages(Messages&& messages) {
  bool success = true;

  for (auto const& message : messages) {
    MachPayload payload(message, _handle);

    if (!payload.send()) {
      success = false;
      break;
    }
  }

  return success ? Result::Success : Result::Failure;
}

MachPort::ReadResult MachPort::readMessages() {
  MachPayload payload(_handle);
  Messages messages;

  if (payload.receive()) {
    messages.emplace_back(std::move(payload.asMessage()));
  }

  return ReadResult(messages.size() == 0 ? Failure : Success,
                    std::move(messages));
}

}  // namespace rl
