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

    case Type::Send:
      /*
       *  Not needed by Radar. So unsupported.
       */
      break;
  }
  //
}

MachPort::MachPort(Type type, mach_port_name_t name)
    : _name(name), _type(type) {
  //
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
    case Type::Send:
      RL_MACH_CHECK(mach_port_mod_refs(mach_task_self(), _name,
                                       MACH_PORT_RIGHT_SEND, -1));
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
  RL_LOG("~~~~~~~~~~~~~~~ Logging Rights for %d", _name);

  struct MachRightNamePair {
    const char* name;
    mach_port_right_t right;
  };

#define __MACH_RIGHT_DESC(a) \
  { #a, a }

  const MachRightNamePair pairs[] = {
      __MACH_RIGHT_DESC(MACH_PORT_RIGHT_SEND),
      __MACH_RIGHT_DESC(MACH_PORT_RIGHT_RECEIVE),
      __MACH_RIGHT_DESC(MACH_PORT_RIGHT_SEND_ONCE),
      __MACH_RIGHT_DESC(MACH_PORT_RIGHT_PORT_SET),
      __MACH_RIGHT_DESC(MACH_PORT_RIGHT_DEAD_NAME),
  };

#undef __MACH_RIGHT_DESC

  const auto count = sizeof(pairs) / sizeof(MachRightNamePair);

  for (size_t i = 0; i < count; i++) {
    mach_port_urefs_t refs = 0;
    kern_return_t result =
        mach_port_get_refs(mach_task_self(), _name, pairs[i].right, &refs);

    if (result == KERN_INVALID_NAME) {
      RL_LOG("This name is invalid");
      break;
    }

    if (result == KERN_SUCCESS) {
      RL_LOG("%30s: %d", pairs[i].name, refs)
      continue;
    }

    RL_LOG_MACHERROR(result);
    break;
  }

  RL_LOG("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~");
}

MachPort::Type MachPort::type() const {
  return _type;
}

mach_port_name_t MachPort::name() const {
  return _name;
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

bool MachPort::isValid() const {
  return MACH_PORT_VALID(_name);
}

Attachment::Handle MachPort::handle() const {
  return _name;
}

}  // namespace core
}  // namespace rl
