// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#pragma once

#include "Config.h"

#if RL_CHANNELS == RL_CHANNELS_MACH

#include <Core/Macros.h>
#include "BootstrapServerProvider.h"

namespace rl {
namespace core {

class MachBootstrapServerProvider : public BootstrapServerProvider {
 public:
  MachBootstrapServerProvider();

  ~MachBootstrapServerProvider();

 private:
  RL_DISALLOW_COPY_AND_ASSIGN(MachBootstrapServerProvider);
};

}  // namespace core
}  // namespace rl

#endif  // RL_CHANNELS == RL_CHANNELS_MACH
