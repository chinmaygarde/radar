/*
 *  This source file is part of the Radar project.
 *  Licensed under the MIT License. See LICENSE file for details.
 */

#pragma once

#include "Config.h"

#if RL_CHANNELS == RL_CHANNELS_SOCKET

#include <Core/Macros.h>
#include "BootstrapClientProvider.h"

namespace rl {
namespace core {

class SocketBootstrapClientProvider : public BootstrapClientProvider {
 public:
  SocketBootstrapClientProvider();

  ~SocketBootstrapClientProvider();

 private:
  IOResult doAdvertise(const std::string& name,
                       std::shared_ptr<Channel> channel) override;

  std::shared_ptr<Channel> doAcquire(const std::string& name) override;

  Message bootstrapServerConnect(const std::string& name);

  Message bootstrapServerConnect(const std::string& name,
                                 AttachmentRef attachment);

  RL_DISALLOW_COPY_AND_ASSIGN(SocketBootstrapClientProvider);
};

}  // namespace core
}  // namespace rl

#endif  //  RL_CHANNELS == RL_CHANNELS_SOCKET
