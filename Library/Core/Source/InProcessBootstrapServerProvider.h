// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef RADAR_CORE_INPROCESSBOOTSTRAPSERVERPROVIDER_H_
#define RADAR_CORE_INPROCESSBOOTSTRAPSERVERPROVIDER_H_

#include "Config.h"

#if RL_CHANNELS == RL_CHANNELS_INPROCESS

#include <Core/Macros.h>

#include "BootstrapServerProvider.h"

namespace rl {
namespace core {

class InProcessBootstrapServerProvider : public BootstrapServerProvider {
 public:
  InProcessBootstrapServerProvider();

  ~InProcessBootstrapServerProvider();

 private:
  RL_DISALLOW_COPY_AND_ASSIGN(InProcessBootstrapServerProvider);
};

}  // namespace core
}  // namespace rl

#endif  // RL_CHANNELS == RL_CHANNELS_INPROCESS

#endif  // RADAR_CORE_INPROCESSBOOTSTRAPSERVERPROVIDER_H_
