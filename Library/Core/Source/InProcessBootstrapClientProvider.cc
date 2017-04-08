/*
 *  This source file is part of the Radar project.
 *  Licensed under the MIT License. See LICENSE file for details.
 */

#include "Config.h"

#if RL_CHANNELS == RL_CHANNELS_INPROCESS

#include "InProcessBootstrapClientProvider.h"

namespace rl {
namespace core {

IOResult InProcessBootstrapClientProvider::doAdvertise(
    const std::string& name,
    std::shared_ptr<Channel> channel) {
  auto registryAccess = _registry.access();
  auto& registry = registryAccess.get();

  auto found = registry.find(name);

  /*
   *  Make sure an entry does not already exist. It it does, the channel for
   *  that should have already expired. If not, a valid channel registration is
   *  is present and this new registration should fail.
   */
  if (found != registry.end() && found->second.lock()) {
    return IOResult::Failure;
  }

  registry[name] = channel;
  return IOResult::Success;
}

std::shared_ptr<Channel> InProcessBootstrapClientProvider::doAcquire(
    const std::string& name) {
  auto registryAccess = _registry.access();
  auto& registry = registryAccess.get();

  auto found = registry.find(name);

  if (found == registry.end()) {
    return nullptr;
  }

  if (auto prototye = found->second.lock()) {
    return prototye;
  } else {
    /*
     *  Between the last time the bootstrap registry was accessed and now,
     *  the channel prototype for this name went away. Get rid of then entry
     *  in our collection. Book-keeping.
     */
    registry.erase(found);
  }

  return nullptr;
}

}  // namespace core
}  // namespace rl

#endif  // RL_CHANNELS == RL_CHANNELS_INPROCESS
