/*
 *  This source file is part of the Radar project.
 *  Licensed under the MIT License. See LICENSE file for details.
 */

#pragma once

#include "Config.h"

#if RL_CHANNELS == RL_CHANNELS_INPROCESS

#include <Core/Macros.h>
#include "BootstrapServerProvider.h"

namespace rl {
namespace core {

class InProcessBootstrapServerProvider : public BootstrapServerProvider {
 public:
  InProcessBootstrapServerProvider();

  ~InProcessBootstrapServerProvider();

 private:
  RL_DISALLOW_COPY_AND_ASSIGN(InProcessBootstrapServerProvider);
};

}  // namespace core
}  // namespace rl

#endif  // RL_CHANNELS == RL_CHANNELS_INPROCESS
