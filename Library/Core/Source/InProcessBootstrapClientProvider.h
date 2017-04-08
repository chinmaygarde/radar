/*
 *  This source file is part of the Radar project.
 *  Licensed under the MIT License. See LICENSE file for details.
 */

#pragma once

#include "Config.h"

#if RL_CHANNELS == RL_CHANNELS_INPROCESS

#include <Core/Channel.h>
#include <Core/Macros.h>
#include <Core/Mutexed.h>
#include <map>
#include "BootstrapClientProvider.h"

namespace rl {
namespace core {

class InProcessBootstrapClientProvider : public BootstrapClientProvider {
 public:
  InProcessBootstrapClientProvider();

  ~InProcessBootstrapClientProvider();

 private:
  using Registry = std::map<std::string, std::weak_ptr<Channel>>;

  Mutexed<Registry> _registry;

  IOResult doAdvertise(const std::string& name,
                       std::shared_ptr<Channel> channel) override;

  std::shared_ptr<Channel> doAcquire(const std::string& name) override;

  RL_DISALLOW_COPY_AND_ASSIGN(InProcessBootstrapClientProvider);
};

}  // namespace core
}  // namespace rl

#endif  // RL_CHANNELS == RL_CHANNELS_INPROCESS
