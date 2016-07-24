// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef RADAR_CORE_INPROCESSBOOTSTRAPCLIENTPROVIDER_H_
#define RADAR_CORE_INPROCESSBOOTSTRAPCLIENTPROVIDER_H_

#include <Core/Config.h>

#if RL_CHANNELS == RL_CHANNELS_INPROCESS

#include <Core/Macros.h>
#include <Core/Mutexed.h>
#include <Core/Channel.h>

#include "BootstrapClientProvider.h"

#include <map>

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

#endif  // RADAR_CORE_INPROCESSBOOTSTRAPCLIENTPROVIDER_H_
