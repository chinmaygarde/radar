/*
 *  This source file is part of the Radar project.
 *  Licensed under the MIT License. See LICENSE file for details.
 */

#pragma once

#include "Config.h"

#if RL_CHANNELS == RL_CHANNELS_MACH

#include <Core/EventLoopSource.h>
#include <Core/Macros.h>
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
