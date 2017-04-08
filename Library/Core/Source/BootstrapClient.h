/*
 *  This source file is part of the Radar project.
 *  Licensed under the MIT License. See LICENSE file for details.
 */

#pragma once

#include <Core/Channel.h>
#include <Core/IOResult.h>
#include <Core/Macros.h>
#include "BootstrapClientProvider.h"

namespace rl {
namespace core {

class BootstrapClient {
 public:
  BootstrapClient();

  ~BootstrapClient();

  IOResult advertise(const std::string& name, std::shared_ptr<Channel> channel);

  std::shared_ptr<Channel> acquire(const std::string& name);

 private:
  std::unique_ptr<BootstrapClientProvider> _provider;

  RL_DISALLOW_COPY_AND_ASSIGN(BootstrapClient);
};

}  // namespace core
}  // namespace rl
