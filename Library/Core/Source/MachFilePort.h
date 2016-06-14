// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef RADAR_CORE_MACHFILEPORT_H_
#define RADAR_CORE_MACHFILEPORT_H_

#include <Core/Macros.h>
#include <mach/mach.h>

namespace rl {
namespace core {
namespace mach {

RL_WARN_UNUSED_RESULT
int FileDescriptorFromMachPort(mach_port_name_t name);

RL_WARN_UNUSED_RESULT
mach_port_name_t MachPortFromFileDescriptor(int descriptor);

}  // namespace mach
}  // namespace core
}  // namespace rl

#endif  // RADAR_CORE_MACHFILEPORT_H_
