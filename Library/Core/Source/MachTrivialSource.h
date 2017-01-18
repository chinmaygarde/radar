// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef RADAR_CORE_MACHTRIVIALSOURCE_H_
#define RADAR_CORE_MACHTRIVIALSOURCE_H_

#include "Config.h"

#if RL_CHANNELS == RL_CHANNELS_MACH

#include <Core/Core.h>
#include "MachPort.h"

namespace rl {
namespace core {

class MachTrivialSource : public EventLoopSource {
 public:
  MachTrivialSource();
  ~MachTrivialSource();

 private:
  MachPort _port;
  MachPort _set;

  RL_DISALLOW_COPY_AND_ASSIGN(MachTrivialSource);
};

}  // namespace core
}  // namespace rl

#endif  // RL_CHANNELS == RL_CHANNELS_MACH

#endif  // RADAR_CORE_MACHTRIVIALSOURCE_H_
