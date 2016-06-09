// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "MachPort.h"

#include "MachPayload.h"

namespace rl {
namespace core {

MachPort::MachPort(Type type) : _name(MACH_PORT_NULL), _type(type) {
  switch (_type) {
    case Type::SendReceive: {
      /*
       *  Allocation the receive port.
       */
      kern_return_t res =
          mach_port_allocate(mach_task_self(), MACH_PORT_RIGHT_RECEIVE, &_name);
      if (res != KERN_SUCCESS) {
        RL_LOG_MACHERROR(res);
        return;
      }

      /*
       *  Insert the send port right.
       */
      res = mach_port_insert_right(mach_task_self(), _name, _name,
                                   MACH_MSG_TYPE_MAKE_SEND);
      if (res != KERN_SUCCESS) {
        /*
         *  Cleanup and deallocate the port of the insertion failed.
         */
        RL_MACH_CHECK(mach_port_deallocate(mach_task_self(), _name));
        _name = MACH_PORT_NULL;
      }
    } break;

    case Type::PortSet: {
      /*
       *  Allocate the port set.
       */
      kern_return_t res = mach_port_allocate(mach_task_self(),
                                             MACH_PORT_RIGHT_PORT_SET, &_name);
      if (res != KERN_SUCCESS) {
        RL_LOG_MACHERROR(res);
        _name = MACH_PORT_NULL;
      }

    } break;
  }
}

MachPort::MachPort(Type type, mach_port_name_t name)
    : _name(name), _type(type) {
  /*
   *  Investigate if we should assume ownership or are already given the same
   *  when the attachment is decoded from the message.
   */
}

MachPort::~MachPort() {
  if (!MACH_PORT_VALID(_name)) {
    return;
  }

  switch (_type) {
    case Type::SendReceive:
      RL_MACH_CHECK(mach_port_mod_refs(mach_task_self(), _name,
                                       MACH_PORT_RIGHT_SEND, -1));
      RL_MACH_CHECK(mach_port_mod_refs(mach_task_self(), _name,
                                       MACH_PORT_RIGHT_RECEIVE, -1));
      break;
    case Type::PortSet:
      RL_MACH_CHECK(mach_port_mod_refs(mach_task_self(), _name,
                                       MACH_PORT_RIGHT_PORT_SET, -1));
      break;
  }
}

bool MachPort::setQueueLimit(size_t limit) {
  if (!isValid()) {
    return false;
  }

  mach_port_limits_t limits = {0};

  limits.mpl_qlimit = static_cast<mach_port_msgcount_t>(limit);

  kern_return_t res =
      mach_port_set_attributes(mach_task_self(),            // task
                               _name,                       // name
                               MACH_PORT_LIMITS_INFO,       // key
                               (mach_port_info_t)&limits,   // value
                               MACH_PORT_LIMITS_INFO_COUNT  // count
                               );

  if (res != KERN_SUCCESS) {
    RL_LOG_MACHERROR(res);
    return false;
  }

  return true;
}

bool MachPort::insertMember(const MachPort& member) {
  if (!isValid() || !member.isValid()) {
    return false;
  }

  kern_return_t res =
      mach_port_insert_member(mach_task_self(), member._name, _name);

  if (res != KERN_SUCCESS) {
    RL_LOG_MACHERROR(res);
    return false;
  }

  return true;
}

bool MachPort::extractMember(const MachPort& member) {
  if (!isValid() || !member.isValid()) {
    return false;
  }

  kern_return_t res =
      mach_port_extract_member(mach_task_self(), member._name, _name);

  if (res != KERN_SUCCESS) {
    RL_LOG_MACHERROR(res);
    return false;
  }

  return true;
}

void MachPort::logRights() const {
  mach_port_name_t name = _name;

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

mach_port_name_t MachPort::name() const {
  return _name;
}

bool MachPort::isValid() const {
  return MACH_PORT_VALID(_name);
}

static inline mach_msg_timeout_t MachMessageTimeOutFromDuration(
    ClockDurationNano timeout) {
  return static_cast<mach_msg_timeout_t>(
      std::chrono::duration_cast<ClockDurationMilli>(timeout).count());
}

IOResult MachPort::sendMessages(Messages&& messages,
                                ClockDurationNano requestedTimeout) const {
  if (!isValid()) {
    return IOResult::Failure;
  }

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
  mach_port_t handle = _name;

  for (auto const& message : messages) {
    MachPayload payload(message, handle);
    result = payload.send(timeoutOption, timeout);

    if (result == IOResult::Failure) {
      return result;
    }
  }

  return result;
}

IOReadResult MachPort::receiveMessage(
    ClockDurationNano requestedTimeout) const {
  MachPayload payload(_name);

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

}  // namespace core
}  // namespace rl
