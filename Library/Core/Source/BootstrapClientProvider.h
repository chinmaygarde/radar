/*
 *  This source file is part of the Radar project.
 *  Licensed under the MIT License. See LICENSE file for details.
 */

#pragma once

#include <Core/Channel.h>
#include <Core/Macros.h>
#include <string>

namespace rl {
namespace core {

class BootstrapClientProvider {
 public:
  virtual ~BootstrapClientProvider();

  virtual IOResult doAdvertise(const std::string& name,
                               std::shared_ptr<Channel> channel) = 0;

  virtual std::shared_ptr<Channel> doAcquire(const std::string& name) = 0;
};

}  // namespace core
}  // namespace rl
