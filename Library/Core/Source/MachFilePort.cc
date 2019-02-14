/*
 *  This source file is part of the Radar project.
 *  Licensed under the MIT License. See LICENSE file for details.
 */

#include "Config.h"

#if RL_CHANNELS == RL_CHANNELS_MACH

#include <sys/syscall.h>
#include <unistd.h>
#include "MachFilePort.h"

#ifndef SYS_fileport_makeport
#error SYS_fileport_makeport not defined.
#endif

#ifndef SYS_fileport_makefd
#error SYS_fileport_makefd not defined.
#endif

namespace rl {
namespace core {

static const int kInvalidDescriptor = -1;

int MachFileDescriptorFromPort(mach_port_name_t name, bool consumePort) {
  if (!MACH_PORT_VALID(name)) {
    return kInvalidDescriptor;
  }

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdeprecated-declarations"
  int result = syscall(SYS_fileport_makefd, name);
#pragma clang diagnostic pop

  if (consumePort) {
    RL_MACH_CHECK(
        mach_port_mod_refs(mach_task_self(), name, MACH_PORT_RIGHT_SEND, -1));
  }

  return result;
}

mach_port_name_t MachPortFromFileDescriptor(int descriptor) {
  if (descriptor == kInvalidDescriptor) {
    return MACH_PORT_NULL;
  }

  mach_port_name_t port = MACH_PORT_NULL;

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdeprecated-declarations"
  int result = syscall(SYS_fileport_makeport, descriptor, &port);
  if (result == 0) {
    return port;
  }
#pragma clang diagnostic pop

  return MACH_PORT_NULL;
}

}  // namespace core
}  // namespace rl

#endif  // RL_CHANNELS == RL_CHANNELS_MACH
