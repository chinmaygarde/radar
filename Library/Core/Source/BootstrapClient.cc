/*
 *  This source file is part of the Radar project.
 *  Licensed under the MIT License. See LICENSE file for details.
 */

#include "BootstrapClient.h"
#include "InProcessBootstrapClientProvider.h"
#include "MachBootstrapClientProvider.h"
#include "SocketBootstrapClientProvider.h"

namespace rl {
namespace core {

using PlatformBootstrapClientProvider =
#if RL_CHANNELS == RL_CHANNELS_MACH
    MachBootstrapClientProvider
#elif RL_CHANNELS == RL_CHANNELS_SOCKET
    SocketBootstrapClientProvider
#elif RL_CHANNELS == RL_CHANNELS_INPROCESS
    InProcessBootstrapClientProvider
#else
#error Unknown Channels Implementation
#endif
    ;

BootstrapClient::BootstrapClient()
    : _provider(std::make_unique<PlatformBootstrapClientProvider>()) {}

BootstrapClient::~BootstrapClient() = default;

IOResult BootstrapClient::advertise(const std::string& name,
                                    std::shared_ptr<Channel> channel) {
  if (name.size() == 0) {
    return IOResult::Failure;
  }

  if (!channel) {
    return IOResult::Failure;
  }

  return _provider->doAdvertise(name, std::move(channel));
}

std::shared_ptr<Channel> BootstrapClient::acquire(const std::string& name) {
  if (name.size() == 0) {
    return nullptr;
  }

  return _provider->doAcquire(name);
}

}  // namespace core
}  // namespace rl
