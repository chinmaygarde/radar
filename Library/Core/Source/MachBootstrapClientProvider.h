/*
 *  This source file is part of the Radar project.
 *  Licensed under the MIT License. See LICENSE file for details.
 */

#pragma once

#include "Config.h"

#if RL_CHANNELS == RL_CHANNELS_MACH

#include <Core/Macros.h>
#include "BootstrapClientProvider.h"

namespace rl {
namespace core {

class MachBootstrapClientProvider : public BootstrapClientProvider {
 public:
  MachBootstrapClientProvider();

  ~MachBootstrapClientProvider();

 private:
  IOResult doAdvertise(const std::string& name,
                       std::shared_ptr<Channel> channel) override;

  std::shared_ptr<Channel> doAcquire(const std::string& name) override;

  RL_DISALLOW_COPY_AND_ASSIGN(MachBootstrapClientProvider);
};

}  // namespace core
}  // namespace rl

#endif  // RL_CHANNELS == RL_CHANNELS_MACH
