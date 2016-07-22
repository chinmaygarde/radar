// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef RADAR_CORE_MACHBOOTSTRAPCLIENTPROVIDER_H_
#define RADAR_CORE_MACHBOOTSTRAPCLIENTPROVIDER_H_

#include <Core/Config.h>

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

#endif  // RADAR_CORE_MACHBOOTSTRAPCLIENTPROVIDER_H_
