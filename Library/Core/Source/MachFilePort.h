// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef RADAR_CORE_MACHFILEPORT_H_
#define RADAR_CORE_MACHFILEPORT_H_

#include "Config.h"

#if RL_CHANNELS == RL_CHANNELS_MACH

#include <Core/Macros.h>
#include <mach/mach.h>

namespace rl {
namespace core {

RL_WARN_UNUSED_RESULT
int MachFileDescriptorFromPort(mach_port_name_t name, bool consumePort);

RL_WARN_UNUSED_RESULT
mach_port_name_t MachPortFromFileDescriptor(int descriptor);

}  // namespace core
}  // namespace rl

#endif  // RL_CHANNELS == RL_CHANNELS_MACH

#endif  // RADAR_CORE_MACHFILEPORT_H_
