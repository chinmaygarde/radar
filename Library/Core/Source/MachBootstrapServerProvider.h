/*
 *  This source file is part of the Radar project.
 *  Licensed under the MIT License. See LICENSE file for details.
 */

#pragma once

#include "Config.h"

#if RL_CHANNELS == RL_CHANNELS_MACH

#include <Core/Macros.h>
#include "BootstrapServerProvider.h"

namespace rl {
namespace core {

class MachBootstrapServerProvider : public BootstrapServerProvider {
 public:
  MachBootstrapServerProvider();

  ~MachBootstrapServerProvider();

 private:
  RL_DISALLOW_COPY_AND_ASSIGN(MachBootstrapServerProvider);
};

}  // namespace core
}  // namespace rl

#endif  // RL_CHANNELS == RL_CHANNELS_MACH
