// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "MachFilePort.h"

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
namespace mach {

int FileDescriptorFromMachPort(mach_port_name_t name) {
  if (!MACH_PORT_VALID(name)) {
    return -1;
  }

  return syscall(SYS_fileport_makefd, name);
}

mach_port_name_t MachPortFromFileDescriptor(int descriptor) {
  if (descriptor == -1) {
    return -1;
  }

  mach_port_name_t port = MACH_PORT_NULL;

  if (syscall(SYS_fileport_makeport, descriptor, &port) == 0) {
    return port;
  }

  return MACH_PORT_NULL;
}

}  // namespace mach
}  // namespace core
}  // namespace rl
