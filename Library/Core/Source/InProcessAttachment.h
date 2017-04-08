/*
 *  This source file is part of the Radar project.
 *  Licensed under the MIT License. See LICENSE file for details.
 */

#pragma once

#include <Core/Config.h>

#if RL_CHANNELS == RL_CHANNELS_INPROCESS

#include <Core/Macros.h>

namespace rl {
namespace core {

class InProcessAttachment {
 public:
  InProcessAttachment();

  virtual ~InProcessAttachment();

 private:
  RL_DISALLOW_COPY_AND_ASSIGN(InProcessAttachment);
};

}  // namespace core
}  // namespace rl

#endif  // RL_CHANNELS == RL_CHANNELS_INPROCESS
