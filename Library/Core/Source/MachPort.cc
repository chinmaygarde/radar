// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <Core/Config.h>

#if RL_CHANNELS == RL_CHANNELS_MACH

#include "MachPort.h"
#include "MachPayload.h"

namespace rl {
namespace core {

MachPort::MachPort(Attachment attachment) {
  RL_ASSERT_MSG(attachment.isValid(),
                "Mach ports can only be created from valid channel handles");

  setupWithPortHandle(static_cast<MachPort::Handle>(attachment.handle()));
}

static MachPort::Handle MachPortCreateSendPortWithLimit(size_t queueLimit) {
  mach_port_name_t handle = MACH_PORT_NULL;

  /*
   *  Step 1: Allocate the port that will be used to send and receive messages
   */
  kern_return_t res =
      mach_port_allocate(mach_task_self(), MACH_PORT_RIGHT_RECEIVE, &handle);
  RL_ASSERT(res == KERN_SUCCESS && handle != MACH_PORT_NULL);

  /*
   *  Step 2: Insert the send port right
   */
  res = mach_port_insert_right(mach_task_self(), handle, handle,
                               MACH_MSG_TYPE_MAKE_SEND);
  RL_ASSERT(res == KERN_SUCCESS);

  /*
   *  Step 3: Set the q limit if it differs from the default
   */
  if (queueLimit != MACH_PORT_QLIMIT_DEFAULT) {
    mach_port_limits_t limits = {0};
    limits.mpl_qlimit = static_cast<mach_port_msgcount_t>(queueLimit);
    res = mach_port_set_attributes(
        mach_task_self(), handle, MACH_PORT_LIMITS_INFO,
        (mach_port_info_t)&limits, MACH_PORT_LIMITS_INFO_COUNT);
    RL_ASSERT(res == KERN_SUCCESS);
  }

  return handle;
}

MachPort::MachPort(size_t queueLimit) {
  setupWithPortHandle(MachPortCreateSendPortWithLimit(queueLimit));
}

void MachPort::setupWithPortHandle(Handle handle) {
  RL_ASSERT(handle != MACH_PORT_NULL);

  mach_port_name_t setHandle = MACH_PORT_NULL;

  /*
   *  Step 1: Allocate the port set that will be used as the observer in the
   *          waitset
   */
  kern_return_t res = mach_port_allocate(mach_task_self(),
                                         MACH_PORT_RIGHT_PORT_SET, &setHandle);
  RL_ASSERT(res == KERN_SUCCESS && setHandle != MACH_PORT_NULL);

  /*
   *  Step 2: Insert the port into the port set. And we are all done.
   */
  res = mach_port_insert_member(mach_task_self(), handle, setHandle);
  RL_ASSERT(res == KERN_SUCCESS);

  _portAttachment = Attachment{handle};
  _setAttachment = Attachment{setHandle};
}

MachPort::~MachPort() {
  /*
   *  `doTerminate()` should have already collected the port
   */
  RL_ASSERT(!_portAttachment.isValid() && !_setAttachment.isValid());
}

const Attachment& MachPort::portAttachment() const {
  return _portAttachment;
}

const Attachment& MachPort::setAttachment() const {
  return _setAttachment;
}

static inline bool MachPortModRef(mach_port_name_t name,
                                  mach_port_right_t right,
                                  mach_port_delta_t delta) {
  kern_return_t result =
      mach_port_mod_refs(mach_task_self(), name, right, delta);
  return result == KERN_SUCCESS;
}

static inline mach_port_urefs_t MachPortGetRef(mach_port_name_t name,
                                               mach_port_right_t right) {
  mach_port_urefs_t refs = 0;
  kern_return_t result =
      mach_port_get_refs(mach_task_self(), name, right, &refs);
  RL_ASSERT(result == KERN_SUCCESS);
  return refs;
}

bool MachPort::doTerminate() {
  auto success = true;

  mach_port_name_t handle = _portAttachment.handle();
  mach_port_name_t setHandle = _setAttachment.handle();

  kern_return_t result =
      mach_port_extract_member(mach_task_self(), handle, setHandle);
  if (result != KERN_SUCCESS && result != KERN_NOT_IN_SET) {
    success = false;
  }

  bool releaseSend = MachPortModRef(handle, MACH_PORT_RIGHT_SEND, -1);
  auto refs = MachPortGetRef(handle, MACH_PORT_RIGHT_SEND);
  bool releaseReceive =
      refs == 0 ? MachPortModRef(handle, MACH_PORT_RIGHT_RECEIVE, -1) : true;

  if (!releaseSend || !releaseReceive) {
    success = false;
  } else {
    _portAttachment.invalidate();
  }

  result = mach_port_destroy(mach_task_self(), setHandle);
  if (result != KERN_SUCCESS) {
    success = false;
  } else {
    _setAttachment.invalidate();
  }

  return success;
}

static inline mach_msg_timeout_t MachMessageTimeOutFromDuration(
    ClockDurationNano timeout) {
  return static_cast<mach_msg_timeout_t>(
      std::chrono::duration_cast<ClockDurationMilli>(timeout).count());
}

IOResult MachPort::sendMessages(Messages&& messages,
                                ClockDurationNano requestedTimeout) {
  mach_msg_option_t timeoutOption = 0;
  mach_msg_timeout_t timeout = MACH_MSG_TIMEOUT_NONE;

  if (requestedTimeout != ClockDurationNano::max()) {
    timeoutOption = MACH_SEND_TIMEOUT;
    timeout = MachMessageTimeOutFromDuration(requestedTimeout);
  }

  auto result = IOResult::Failure;
  /*
   *  This is incorrect. Multiple messages need to be sent in one call
   */
  mach_port_t handle = _portAttachment.handle();

  for (auto const& message : messages) {
    MachPayload payload(message, handle);
    result = payload.send(timeoutOption, timeout);

    if (result == IOResult::Failure) {
      return result;
    }
  }

  return result;
}

IOReadResult MachPort::readMessage(ClockDurationNano requestedTimeout) {
  MachPayload payload(_portAttachment.handle());

  mach_msg_option_t timeoutOption = 0;
  mach_msg_timeout_t timeout = MACH_MSG_TIMEOUT_NONE;

  auto hasTimeout = requestedTimeout != ClockDurationNano::max();

  if (hasTimeout) {
    timeoutOption = MACH_RCV_TIMEOUT;
    timeout = MachMessageTimeOutFromDuration(requestedTimeout);
  }

  auto result = payload.receive(timeoutOption, timeout);

  if (result == IOResult::Success) {
    return IOReadResult(result, payload.asMessage());
  }

  return IOReadResult(result, Message{});
}

void MachPort::LogRights(Handle name) {
  kern_return_t result = KERN_SUCCESS;
  mach_port_urefs_t refs = 0;

  RL_LOG("~~~~~~~~~~ Logging Rights for %d", name);

  refs = 0;
  result =
      mach_port_get_refs(mach_task_self(), name, MACH_PORT_RIGHT_SEND, &refs);

  if (result == KERN_INVALID_NAME) {
    RL_LOG("This name is invalid");
    return;
  }

  RL_ASSERT(result == KERN_SUCCESS);
  RL_LOG("Send %d", refs);

  refs = 0;
  result = mach_port_get_refs(mach_task_self(), name, MACH_PORT_RIGHT_RECEIVE,
                              &refs);
  RL_ASSERT(result == KERN_SUCCESS);
  RL_LOG("Receive %d", refs);

  refs = 0;
  result = mach_port_get_refs(mach_task_self(), name, MACH_PORT_RIGHT_SEND_ONCE,
                              &refs);
  RL_ASSERT(result == KERN_SUCCESS);
  RL_LOG("Send Once %d", refs);

  refs = 0;
  result = mach_port_get_refs(mach_task_self(), name, MACH_PORT_RIGHT_PORT_SET,
                              &refs);
  RL_ASSERT(result == KERN_SUCCESS);
  RL_LOG("Port Set %d", refs);

  refs = 0;
  result = mach_port_get_refs(mach_task_self(), name, MACH_PORT_RIGHT_DEAD_NAME,
                              &refs);
  RL_ASSERT(result == KERN_SUCCESS);
  RL_LOG("Dead %d", refs);
}

}  // namespace core
}  // namespace rl

#endif  // RL_CHANNELS == RL_CHANNELS_MACH
