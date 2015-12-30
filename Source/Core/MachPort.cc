// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <Core/Config.h>

#if RL_CHANNELS == RL_CHANNELS_MACH

#include "MachPort.h"

#include <mach/mach.h>

namespace rl {
namespace core {

static inline size_t MachMessageSize(bool hasMemoryArena,
                                     size_t ports,
                                     bool hasTrailer) {
  /*
   *  All messages have a header and a description detailing the number of OOL
   *  descriptors within the message.
   */
  size_t messageSize = sizeof(mach_msg_header_t) + sizeof(mach_msg_body_t);

  /*
   *  An optional memory arena
   */
  if (hasMemoryArena) {
    messageSize += sizeof(mach_msg_ool_descriptor_t);
  }

  /*
   *  A variable number of attachments for the ports
   */
  messageSize += ports * sizeof(mach_msg_port_descriptor_t);

  /*
   *  A trailer on receive
   */
  if (hasTrailer) {
    messageSize += MACH_MSG_TRAILER_MINIMUM_SIZE;
  }

  return messageSize;
}

/**
 *  A MachPayload takes an instance of a `core::Message` and intializes itself
 *  so that it can be cast to a mac_msg_header_t suitable for use with a
 *  mach_msg call.
 */
class MachPayload {
 public:
  /**
   *  Initialize the mach message payload when sending a message to a remote
   *  port
   *
   *  @param message the message to send
   *  @param remote  the port to send the message to
   *
   *  @return the initialized mach message payload for the message and port
   */
  explicit MachPayload(const Message& message, mach_port_t remote) {
    /*
     *  Figure out the total size of the mach message.
     */
    const bool hasMemoryArena = message.size() > 0;
    const size_t ports = message.attachments().size();

    const size_t messageSize = MachMessageSize(hasMemoryArena, ports, false);

    const size_t totalOOLDescriptors = (hasMemoryArena ? 1 : 0) + ports;

    /*
     *  Allocate the mach message payload in one shot
     */
    _payload = reinterpret_cast<uint8_t*>(calloc(messageSize, sizeof(uint8_t)));

    auto offset = 0;

    /*
     *  Initialize the mach message header
     */
    auto header = reinterpret_cast<mach_msg_header_t*>(_payload + offset);
    header->msgh_size = static_cast<mach_msg_size_t>(messageSize);
    header->msgh_remote_port = remote;
    header->msgh_bits =
        MACH_MSGH_BITS_REMOTE(MACH_MSG_TYPE_COPY_SEND) | MACH_MSGH_BITS_COMPLEX;

    offset += sizeof(mach_msg_header_t);

    /*
     *  Initialize the mach message body header.
     *  This has one field that denotes the number of OOL descriptors.
     */
    auto bodyHeader = reinterpret_cast<mach_msg_body_t*>(_payload + offset);
    bodyHeader->msgh_descriptor_count =
        static_cast<mach_msg_size_t>(totalOOLDescriptors);

    offset += sizeof(mach_msg_body_t);

    /*
     *  Initialize OOL memory arena descriptors
     */
    if (hasMemoryArena) {
      auto memDesc =
          reinterpret_cast<mach_msg_ool_descriptor_t*>(_payload + offset);

      memDesc->address = message.data();
      memDesc->size = static_cast<mach_msg_size_t>(message.size());
      memDesc->deallocate = false;
      memDesc->copy = MACH_MSG_VIRTUAL_COPY;
      memDesc->type = MACH_MSG_OOL_DESCRIPTOR;

      offset += sizeof(mach_msg_ool_descriptor_t);
    }

    /*
     *  Initialize port attachment descriptors
     */
    for (const auto& attachment : message.attachments()) {
      auto attachmentDesc =
          reinterpret_cast<mach_msg_port_descriptor_t*>(_payload + offset);

      attachmentDesc->name = static_cast<mach_port_t>(attachment.handle());
      attachmentDesc->disposition = MACH_MSG_TYPE_COPY_SEND;
      attachmentDesc->type = MACH_MSG_PORT_DESCRIPTOR;

      offset += sizeof(mach_msg_port_descriptor_t);
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
  explicit MachPayload(mach_port_t local) {
    /*
     *  The most common message configuration is with one memory arena and no
     *  descriptors. Initialize the payload for that.
     */
    auto messageSize = MachMessageSize(true, 0, true);

    _payload = reinterpret_cast<uint8_t*>(calloc(messageSize, sizeof(char)));

    auto header = reinterpret_cast<mach_msg_header_t*>(_payload);
    header->msgh_size = static_cast<mach_msg_size_t>(messageSize);
    header->msgh_local_port = local;
  }

  IOResult send(mach_msg_option_t timeoutOption, mach_msg_timeout_t timeout) {
    const auto header = reinterpret_cast<mach_msg_header_t*>(_payload);

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
        return IOResult::Success;
      case MACH_SEND_TIMED_OUT:
        return IOResult::Timeout;
      default:
        RL_LOG("Mach Send Failed: %s", mach_error_string(res));
        return IOResult::Failure;
    }

    return IOResult::Failure;
  }

  IOResult receive(mach_msg_option_t timeoutOption,
                   mach_msg_timeout_t timeout) {
    auto header = reinterpret_cast<mach_msg_header_t*>(_payload);
    auto localPort = header->msgh_local_port;

    auto res = MACH_MSG_SUCCESS;

    while (true) {
      // clang-format off
      res = mach_msg(header,
                     MACH_RCV_MSG | MACH_RCV_LARGE | timeoutOption,
                     0,
                     header->msgh_size,
                     header->msgh_local_port,
                     timeout,
                     MACH_PORT_NULL);
      // clang-format on

      if (res == MACH_RCV_TOO_LARGE) {
        const auto reallocSize =
            round_msg(header->msgh_size + MAX_TRAILER_SIZE);

        _payload = reinterpret_cast<uint8_t*>(reallocf(_payload, reallocSize));

        if (_payload == nullptr) {
          /*
           *  We ran out of memory but the message is still in the kernel buffer
           *  because of the MACH_RCV_LARGE. We may be able to service this
           *  read later.
           */
          return IOResult::Timeout;
        }

        header = reinterpret_cast<mach_msg_header_t*>(_payload);
        header->msgh_size = static_cast<mach_msg_size_t>(reallocSize);
        header->msgh_local_port = localPort;

        continue;
      }

      break;
    }

    switch (res) {
      case MACH_MSG_SUCCESS:
        RL_ASSERT(MACH_MSGH_BITS_IS_COMPLEX(header->msgh_bits));
        return IOResult::Success;
      case MACH_RCV_TIMED_OUT:
        return IOResult::Timeout;
      default:
        RL_LOG("Mach Recv Failed: %s", mach_error_string(res));
        return IOResult::Failure;
    }

    return IOResult::Failure;
  }

  Message asMessage() const {
    /*
     *  Read the total number of descriptors available
     */

    auto body = reinterpret_cast<mach_msg_body_t*>(_payload +
                                                   sizeof(mach_msg_header_t));

    auto descriptors = body->msgh_descriptor_count;

    if (descriptors == 0) {
      return Message{};
    }

    uint8_t* memoryArenaAddress = nullptr;
    size_t memoryArenaSize = 0;
    std::vector<Message::Attachment> attachments;

    size_t offset = sizeof(mach_msg_header_t) + sizeof(mach_msg_body_t);

    for (size_t i = 0; i < descriptors; i++) {
      /*
       *  Cast each descriptor to a 'type' kind first to determine the type of
       *  OOL descriptor we are dealing with
       */
      auto typeKind =
          reinterpret_cast<mach_msg_type_descriptor_t*>(_payload + offset);

      /*
       *  Now, cast to the correct type and interpret the message
       */
      switch (typeKind->type) {
        /*
         *  An OOL memory arena
         */
        case MACH_MSG_OOL_DESCRIPTOR: {
          /*
           *  Since we dont support multiple memory arenas per message, assert
           *  that we dont get multiple OOL memory descriptors (ports are fine).
           */
          RL_ASSERT(memoryArenaAddress == nullptr);
          auto mem =
              reinterpret_cast<mach_msg_ool_descriptor_t*>(_payload + offset);

          memoryArenaAddress = static_cast<uint8_t*>(mem->address);
          memoryArenaSize = mem->size;

          offset += sizeof(mach_msg_ool_descriptor_t);
        } break;
        /*
         *  An OOL port
         */
        case MACH_MSG_PORT_DESCRIPTOR: {
          auto attachmentDesc =
              reinterpret_cast<mach_msg_port_descriptor_t*>(_payload + offset);

          attachments.emplace_back(attachmentDesc->name);

          offset += sizeof(mach_msg_port_descriptor_t);
        } break;
        default:
          RL_ASSERT_MSG(
              false, "Unsupported OOL descriptor (not a port or memory arena)");
          break;
      }
    }

    Message message(memoryArenaAddress, memoryArenaSize);
    message.setAttachments(std::move(attachments));
    return message;
  }

  ~MachPayload() { free(_payload); }

 private:
  uint8_t* _payload;

  RL_DISALLOW_COPY_AND_ASSIGN(MachPayload);
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

  kern_return_t result =
      mach_port_extract_member(mach_task_self(), _handle, _setHandle);
  if (result != KERN_SUCCESS && result != KERN_NOT_IN_SET) {
    success = false;
  }

  bool releaseSend = MachPortModRef(_handle, MACH_PORT_RIGHT_SEND, -1);
  auto refs = MachPortGetRef(_handle, MACH_PORT_RIGHT_SEND);
  bool releaseReceive =
      refs == 0 ? MachPortModRef(_handle, MACH_PORT_RIGHT_RECEIVE, -1) : true;

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
  for (auto const& message : messages) {
    MachPayload payload(message, _handle);
    result = payload.send(timeoutOption, timeout);

    if (result == IOResult::Failure) {
      return result;
    }
  }

  return result;
}

IOReadResult MachPort::readMessage(ClockDurationNano requestedTimeout) {
  MachPayload payload(_handle);

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
