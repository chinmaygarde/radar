// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <Core/Config.h>

#if RL_CHANNELS == RL_CHANNELS_MACH

#include "MachPort.h"

#include <mach/mach.h>

namespace rl {
namespace core {

enum class MachPayloadKind : mach_msg_id_t {
  Data = 1,
  Port,
};

/**
 *  A MachPayload takes an instance of a `core::Message` and intializes itself
 *  so that it can be cast to a mac_msg_header_t suitable for use with a
 *  mach_msg call.
 */
struct MachPayload {
  mach_msg_header_t header;
  /*
   *  All mach payloads are complex with one descriptor
   */
  mach_msg_body_t type;
  union {
    mach_msg_ool_descriptor_t memory;
  } body;

  mach_msg_trailer_info_t trailer;

  /**
   *  Initialize the mach message payload when sending a message to a remote
   *  port
   *
   *  @param message the message to send
   *  @param remote  the port to send the message to
   *
   *  @return the initialized mach message payload for the message and port
   */
  explicit MachPayload(const Message& message, mach_port_t remote)
      : header({
            .msgh_size = sizeof(MachPayload) - sizeof(trailer),
            .msgh_remote_port = remote,
            .msgh_bits = MACH_MSGH_BITS_REMOTE(MACH_MSG_TYPE_COPY_SEND) |
                         MACH_MSGH_BITS_COMPLEX,
        }),
        type({.msgh_descriptor_count = 1}),
        trailer() {
    if (message.attachment().isValid()) {
      /*
       *  Configure this message as an out of line port descriptor
       */
      header.msgh_id = static_cast<mach_msg_id_t>(MachPayloadKind::Port);
      RL_ASSERT(false);  // WIP
    } else {
      /*
       *  Configure this message as an out of line memory descriptor
       */
      header.msgh_id = static_cast<mach_msg_id_t>(MachPayloadKind::Data);
      body.memory = (const mach_msg_ool_descriptor_t){
          .address = message.data(),
          .size = static_cast<mach_msg_size_t>(message.size()),
          .deallocate = false,
          .copy = MACH_MSG_VIRTUAL_COPY,
          .type = MACH_MSG_OOL_DESCRIPTOR,
      };
    }
  }

  /**
   *  Initialize the mach message payload for receiving messages on a local
   *  port
   *
   *  @param local the local port to receive the message on
   *
   *  @return the initialized mach message payload for recieving the message
   */
  explicit MachPayload(mach_port_t local)
      : header({.msgh_size = sizeof(MachPayload), .msgh_local_port = local}),
        type(),
        body(),
        trailer() {}

  EventLoopSource::IOHandlerResult send(mach_msg_option_t timeoutOption,
                                        mach_msg_timeout_t timeout) {
    const auto header = reinterpret_cast<mach_msg_header_t*>(this);

    // clang-format off
    auto res = mach_msg(header,
                        MACH_SEND_MSG | timeoutOption,
                        /* The msgh_size in the header is ignored */
                        header->msgh_size,
                        0,
                        MACH_PORT_NULL,
                        timeout,
                        MACH_PORT_NULL);
    // clang-format on

    switch (res) {
      case MACH_MSG_SUCCESS:
        return EventLoopSource::IOHandlerResult::Success;
      case MACH_SEND_TIMED_OUT:
        return EventLoopSource::IOHandlerResult::Timeout;
      default:
        RL_LOG("Mach Send Failed: %s", mach_error_string(res));
        return EventLoopSource::IOHandlerResult::Failure;
    }

    return EventLoopSource::IOHandlerResult::Failure;
  }

  EventLoopSource::IOHandlerResult receive(mach_msg_option_t timeoutOption,
                                           mach_msg_timeout_t timeout) {
    const auto header = reinterpret_cast<mach_msg_header_t*>(this);

    // clang-format off
    auto res = mach_msg(header,
                        MACH_RCV_MSG | timeoutOption,
                        0,
                        header->msgh_size,
                        header->msgh_local_port,
                        timeout,
                        MACH_PORT_NULL);
    // clang-format on

    switch (res) {
      case MACH_MSG_SUCCESS:
        return EventLoopSource::IOHandlerResult::Success;
      case MACH_RCV_TIMED_OUT:
        return EventLoopSource::IOHandlerResult::Timeout;
      default:
        RL_LOG("Mach Recv Failed: %s", mach_error_string(res));
        return EventLoopSource::IOHandlerResult::Failure;
    }

    return EventLoopSource::IOHandlerResult::Failure;
  }

  MachPayloadKind kind() const {
    return static_cast<MachPayloadKind>(header.msgh_id);
  }

  Message asMessage() const {
    switch (kind()) {
      case MachPayloadKind::Data:
        return Message(static_cast<uint8_t*>(body.memory.address),
                       body.memory.size, true);
      case MachPayloadKind::Port:
        RL_ASSERT(false);  // WIP
        return Message(0, 0);
    }
  }
};

MachPort::MachPort(size_t queueLimit) {
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
  limits.mpl_qlimit = static_cast<mach_port_msgcount_t>(queueLimit);
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

static inline mach_msg_timeout_t MachMessageTimeOutFromDuration(
    ClockDurationNano timeout) {
  return static_cast<mach_msg_timeout_t>(
      std::chrono::duration_cast<ClockDurationMilli>(timeout).count());
}

EventLoopSource::IOHandlerResult MachPort::sendMessages(
    Messages&& messages,
    ClockDurationNano requestedTimeout) {
  mach_msg_option_t timeoutOption = 0;
  mach_msg_timeout_t timeout = MACH_MSG_TIMEOUT_NONE;

  if (requestedTimeout != ClockDurationNano::max()) {
    timeoutOption = MACH_SEND_TIMEOUT;
    timeout = MachMessageTimeOutFromDuration(requestedTimeout);
  }

  auto result = EventLoopSource::IOHandlerResult::Failure;
  /*
   *  This is incorrect. Multiple messages need to be sent in one call
   */
  for (auto const& message : messages) {
    MachPayload payload(message, _handle);
    result = payload.send(timeoutOption, timeout);

    if (result == EventLoopSource::IOHandlerResult::Failure) {
      return result;
    }
  }

  return result;
}

MachPort::ReadResult MachPort::readMessage(ClockDurationNano requestedTimeout) {
  MachPayload payload(_handle);

  mach_msg_option_t timeoutOption = 0;
  mach_msg_timeout_t timeout = MACH_MSG_TIMEOUT_NONE;

  auto hasTimeout = requestedTimeout != ClockDurationNano::max();

  if (hasTimeout) {
    timeoutOption = MACH_RCV_TIMEOUT;
    timeout = MachMessageTimeOutFromDuration(requestedTimeout);
  }

  auto result = payload.receive(timeoutOption, timeout);

  if (result == EventLoopSource::IOHandlerResult::Success) {
    return ReadResult(result, std::move(payload.asMessage()));
  }

  Message empty;
  return ReadResult(result, std::move(empty));
}

}  // namespace core
}  // namespace rl

#endif  // RL_CHANNELS == RL_CHANNELS_MACH
