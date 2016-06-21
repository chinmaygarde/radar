// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "MachFilePort.h"

#include <mach/mach.h>
#include <sys/syscall.h>
#include <unistd.h>

#ifndef SYS_fileport_makeport
#error SYS_fileport_makeport not defined.
#endif

#ifndef SYS_fileport_makefd
#error SYS_fileport_makefd not defined.
#endif

namespace rl {
namespace core {

static const int kInvalidDescriptor = -1;

int MachFilePort::DescriptorFromPort(Name name, bool consumePort) {
  if (!MACH_PORT_VALID(name)) {
    return kInvalidDescriptor;
  }

  int result = syscall(SYS_fileport_makefd, name);

  if (consumePort) {
    RL_MACH_CHECK(
        mach_port_mod_refs(mach_task_self(), name, MACH_PORT_RIGHT_SEND, -1));
  }

  return result;
}

RL_WARN_UNUSED_RESULT
static mach_port_name_t MachPortFromFileDescriptor(int descriptor) {
  if (descriptor == kInvalidDescriptor) {
    return MACH_PORT_NULL;
  }

  mach_port_name_t port = MACH_PORT_NULL;

  if (syscall(SYS_fileport_makeport, descriptor, &port) == 0) {
    return port;
  }

  return MACH_PORT_NULL;
}

MachFilePort::MachFilePort(int descriptor)
    : _name(MachPortFromFileDescriptor(descriptor)) {}

MachFilePort::~MachFilePort() {
  if (MACH_PORT_VALID(_name)) {
    RL_MACH_CHECK(
        mach_port_mod_refs(mach_task_self(), _name, MACH_PORT_RIGHT_SEND, -1));
  }

  _name = MACH_PORT_NULL;
}

MachFilePort::Name MachFilePort::name() const {
  return _name;
}

}  // namespace core
}  // namespace rl
