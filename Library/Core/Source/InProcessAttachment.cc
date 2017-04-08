/*
 *  This source file is part of the Radar project.
 *  Licensed under the MIT License. See LICENSE file for details.
 */

#include "Config.h"

#if RL_CHANNELS == RL_CHANNELS_INPROCESS

#include "InProcessAttachment.h"

namespace rl {
namespace core {

InProcessAttachment::InProcessAttachment() {}

InProcessAttachment::~InProcessAttachment() {}

}  // namespace core
}  // namespace rl

#endif  // RL_CHANNELS == RL_CHANNELS_INPROCESS
