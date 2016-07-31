// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "Config.h"

#if RL_CHANNELS == RL_CHANNELS_MACH

#include "MachPayload.h"

#include "MachPort.h"

namespace rl {
namespace core {

static size_t MachMessageSize(bool hasMemoryArena,
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

MachPayload::MachPayload(const Message& message, mach_port_t remote) {
  /*
   *  Figure out the total size of the mach message.
   */
  const bool hasMemoryArena = message.size() > 0;

  const size_t ports = message.attachmentsSize();

  const size_t messageSize = MachMessageSize(hasMemoryArena, ports, false);

  const size_t totalOOLDescriptors = (hasMemoryArena ? 1 : 0) + ports;

  /*
   *  Allocate the mach message payload in one shot
   */
  if (!_payload.resize(messageSize)) {
    return;
  }

  auto payload = _payload.data();

  auto offset = 0;

  /*
   *  Initialize the mach message header
   */
  auto header = reinterpret_cast<mach_msg_header_t*>(payload + offset);
  header->msgh_size = static_cast<mach_msg_size_t>(messageSize);
  header->msgh_remote_port = remote;
  header->msgh_bits =
      MACH_MSGH_BITS_REMOTE(MACH_MSG_TYPE_COPY_SEND) | MACH_MSGH_BITS_COMPLEX;

  offset += sizeof(mach_msg_header_t);

  /*
   *  Initialize the mach message body header.
   *  This has one field that denotes the number of OOL descriptors.
   */
  auto bodyHeader = reinterpret_cast<mach_msg_body_t*>(payload + offset);
  bodyHeader->msgh_descriptor_count =
      static_cast<mach_msg_size_t>(totalOOLDescriptors);

  offset += sizeof(mach_msg_body_t);

  /*
   *  Initialize OOL memory arena descriptors
   */
  if (hasMemoryArena) {
    auto memDesc =
        reinterpret_cast<mach_msg_ool_descriptor_t*>(payload + offset);

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
        reinterpret_cast<mach_msg_port_descriptor_t*>(payload + offset);

    attachmentDesc->name =
        static_cast<mach_port_t>(attachment->attachmentHandle());
    attachmentDesc->disposition = MACH_MSG_TYPE_COPY_SEND;
    attachmentDesc->type = MACH_MSG_PORT_DESCRIPTOR;

    offset += sizeof(mach_msg_port_descriptor_t);
  }
}

MachPayload::MachPayload(mach_port_t local) {
  /*
   *  The most common message configuration is with one memory arena and no
   *  descriptors. Initialize the payload for that.
   */
  auto messageSize = MachMessageSize(true, 0, true);

  if (!_payload.resize(messageSize)) {
    return;
  }

  auto payload = _payload.data();

  auto header = reinterpret_cast<mach_msg_header_t*>(payload);
  header->msgh_size = static_cast<mach_msg_size_t>(messageSize);
  header->msgh_local_port = local;
}

IOResult MachPayload::send(mach_msg_option_t timeoutOption,
                           mach_msg_timeout_t timeout) {
  if (!_payload.isReady()) {
    return IOResult::Failure;
  }

  auto payload = _payload.data();

  const auto header = reinterpret_cast<mach_msg_header_t*>(payload);

  auto res = mach_msg(header,                         // msg
                      MACH_SEND_MSG | timeoutOption,  // option
                      /* The msgh_size in the header is ignored */
                      header->msgh_size,  // send_size
                      0,                  // receive_limit
                      MACH_PORT_NULL,     // receive_name
                      timeout,            // timeout
                      MACH_PORT_NULL      // notify
                      );

  switch (res) {
    case MACH_MSG_SUCCESS:
      return IOResult::Success;
    case MACH_SEND_TIMED_OUT:
      return IOResult::Timeout;
    default:
      RL_LOG_MACHERROR(res);
      return IOResult::Failure;
  }

  return IOResult::Failure;
}

IOResult MachPayload::receive(mach_msg_option_t timeoutOption,
                              mach_msg_timeout_t timeout) {
  if (!_payload.isReady()) {
    return IOResult::Failure;
  }

  auto payload = _payload.data();

  auto header = reinterpret_cast<mach_msg_header_t*>(payload);
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
      const auto reallocSize = round_msg(header->msgh_size + MAX_TRAILER_SIZE);

      if (!_payload.resize(reallocSize)) {
        /*
         *  We ran out of memory but the message is still in the kernel buffer
         *  because of the MACH_RCV_LARGE. We may be able to service this
         *  read later.
         */
        return IOResult::Timeout;
      }

      payload = _payload.data();

      header = reinterpret_cast<mach_msg_header_t*>(payload);
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
      RL_LOG_MACHERROR(res);
      return IOResult::Failure;
  }

  return IOResult::Failure;
}

Message MachPayload::asMessage() const {
  /*
   *  Read the total number of descriptors available
   */
  if (!_payload.isReady()) {
    return Message{};
  }

  auto payload = _payload.data();

  auto body =
      reinterpret_cast<mach_msg_body_t*>(payload + sizeof(mach_msg_header_t));

  auto descriptors = body->msgh_descriptor_count;

  if (descriptors == 0) {
    return Message{};
  }

  uint8_t* memoryArenaAddress = nullptr;
  size_t memoryArenaSize = 0;
  std::vector<AttachmentRef> attachments;

  size_t offset = sizeof(mach_msg_header_t) + sizeof(mach_msg_body_t);

  for (size_t i = 0; i < descriptors; i++) {
    /*
     *  Cast each descriptor to a 'type' kind first to determine the type of
     *  OOL descriptor we are dealing with
     */
    auto typeKind =
        reinterpret_cast<mach_msg_type_descriptor_t*>(payload + offset);

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
            reinterpret_cast<mach_msg_ool_descriptor_t*>(payload + offset);

        memoryArenaAddress = static_cast<uint8_t*>(mem->address);
        memoryArenaSize = mem->size;

        offset += sizeof(mach_msg_ool_descriptor_t);
      } break;
      /*
       *  An OOL port
       */
      case MACH_MSG_PORT_DESCRIPTOR: {
        auto attachmentDesc =
            reinterpret_cast<mach_msg_port_descriptor_t*>(payload + offset);

        attachments.emplace_back(std::make_shared<MachPort>(
            MachPort::Type::SendReceive, attachmentDesc->name));

        offset += sizeof(mach_msg_port_descriptor_t);
      } break;
      default:
        RL_ASSERT_MSG(
            false, "Unsupported OOL descriptor (not a port or memory arena)");
        break;
    }
  }

  Message message(memoryArenaAddress, memoryArenaSize);
  for (auto attachment : attachments) {
    if (!message.encode(std::move(attachment))) {
      RL_ASSERT_MSG(false, "Internal error: Will leak handles in message.");
      return Message{};
    }
  }
  return message;
}

MachPayload::~MachPayload() = default;

}  // namespace core
}  // namespace rl

#endif  // RL_CHANNELS == RL_CHANNELS_MACH
