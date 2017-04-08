/*
 *  This source file is part of the Radar project.
 *  Licensed under the MIT License. See LICENSE file for details.
 */

#pragma once

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
