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
    mach_msg_port_descriptor_t port;
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
      body.port = (const mach_msg_port_descriptor_t){
          .name = static_cast<mach_port_t>(message.attachment().handle()),
          .disposition = MACH_MSG_TYPE_MAKE_SEND,
          .type = MACH_MSG_PORT_DESCRIPTOR,
      };
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
        RL_ASSERT(MACH_MSGH_BITS_IS_COMPLEX(header->msgh_bits));
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
        return Message(Message::Attachment{body.port.name});
    }
  }
};

MachPort::MachPort(const Message::Attachment& attachment) {
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

  _handle = handle;

  /*
   *  Step 1: Allocate the port set that will be used as the observer in the
   *          waitset
   */
  kern_return_t res = mach_port_allocate(mach_task_self(),
                                         MACH_PORT_RIGHT_PORT_SET, &_setHandle);
  RL_ASSERT(res == KERN_SUCCESS && _setHandle != MACH_PORT_NULL);

  /*
   *  Step 2: Insert the port into the port set. And we are all done.
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

static inline bool MachPortModRef(mach_port_name_t name,
                                  mach_port_right_t right,
                                  mach_port_delta_t delta) {
  kern_return_t result =
      mach_port_mod_refs(mach_task_self(), name, right, delta);
  return result == KERN_SUCCESS;
}

bool MachPort::doTerminate() {
  auto success = true;

  kern_return_t result =
      mach_port_extract_member(mach_task_self(), _handle, _setHandle);
  if (result != KERN_SUCCESS) {
    success = false;
  }

  bool releaseSend = MachPortModRef(_handle, MACH_PORT_RIGHT_SEND, -1);
  bool releaseReceive = MachPortModRef(_handle, MACH_PORT_RIGHT_RECEIVE, -1);
  if (!releaseSend || !releaseReceive) {
    success = false;
  } else {
    _handle = MACH_PORT_NULL;
  }

  result = mach_port_destroy(mach_task_self(), _setHandle);
  if (result != KERN_SUCCESS) {
    success = false;
  } else {
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
