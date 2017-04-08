/*
 *  This source file is part of the Radar project.
 *  Licensed under the MIT License. See LICENSE file for details.
 */

#include "Config.h"

#if RL_CHANNELS == RL_CHANNELS_MACH

#include "MachBootstrapServerProvider.h"

namespace rl {
namespace core {

MachBootstrapServerProvider::MachBootstrapServerProvider() = default;

MachBootstrapServerProvider::~MachBootstrapServerProvider() = default;

/*
 *  Nothing much to do here since we are piggy-backing off of the platform
 *  bootstrap server.
 */

}  // namespace core
}  // namespace rl

#endif  // RL_CHANNELS == RL_CHANNELS_MACH
