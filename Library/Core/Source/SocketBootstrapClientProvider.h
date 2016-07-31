// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef RADAR_CORE_SOCKETBOOTSTRAPCLIENTPROVIDER_H_
#define RADAR_CORE_SOCKETBOOTSTRAPCLIENTPROVIDER_H_

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

  RL_DISALLOW_COPY_AND_ASSIGN(SocketBootstrapClientProvider);
};

}  // namespace core
}  // namespace rl

#endif  //  RL_CHANNELS == RL_CHANNELS_SOCKET

#endif  // RADAR_CORE_SOCKETBOOTSTRAPCLIENTPROVIDER_H_
