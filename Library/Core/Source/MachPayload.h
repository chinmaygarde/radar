/*
 *  This source file is part of the Radar project.
 *  Licensed under the MIT License. See LICENSE file for details.
 */

#pragma once

#include "Config.h"

#if RL_CHANNELS == RL_CHANNELS_MACH

#include <Core/Allocation.h>
#include <Core/IOResult.h>
#include <Core/Macros.h>
#include <Core/Message.h>
#include <mach/mach.h>

namespace rl {
namespace core {

/**
 *  A MachPayload takes an instance of a `core::Message` and performs
 *  appropriate initiatialization to make the message be sent or received over
 *  a `mach_msg` call.
 */
class MachPayload {
 public:
  /**
   *  Initialize the mach message payload when sending a message to a remote
   *  port
   *
   *  @param message the message to send
   *  @param remote  the port to send the message to
   */
  MachPayload(const Message& message, mach_port_t remote);

  /**
   *  Initialize the mach message payload for receiving messages on a local
   *  port
   *
   *  @param local the local port to receive the message on
   */
  MachPayload(mach_port_t local);

  ~MachPayload();

  IOResult send(mach_msg_option_t timeoutOption, mach_msg_timeout_t timeout);

  IOResult receive(mach_msg_option_t timeoutOption, mach_msg_timeout_t timeout);

  Message asMessage() const;

 private:
  Allocation _payload;
  std::vector<Attachment::MessageHandle> _messageHandles;

  RL_DISALLOW_COPY_AND_ASSIGN(MachPayload);
};

}  // namespace core
}  // namespace rl

#endif  // RL_CHANNELS == RL_CHANNELS_MACH
