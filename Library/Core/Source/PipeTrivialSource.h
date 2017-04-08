/*
 *  This source file is part of the Radar project.
 *  Licensed under the MIT License. See LICENSE file for details.
 */

#pragma once

#include "Config.h"

#if !RL_OS_WINDOWS

#include <Core/EventLoopSource.h>

namespace rl {
namespace core {

/**
 *  Makes an event loop source instance that is backed by a simple unix pipe.
 *  This source is only meant to be used on platforms that do not have
 *  coalesce-able userspace signalling of descriptors. This is because
 *  the read end will have to perform multiple reads to service a signal.
 *  Bottom line, use this as a last resort. Basically any other trivial source
 *  types will do.
 *
 *  @return the trivial event loop source
 */
std::shared_ptr<EventLoopSource> MakePipeBasedTrivialSource();

}  // namespace core
}  // namespace rl

#endif  // !RL_OS_WINDOWS
