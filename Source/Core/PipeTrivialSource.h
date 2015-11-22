// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef RADARLOVE_CORE_PIPETRIVIALSOURCE_H_
#define RADARLOVE_CORE_PIPETRIVIALSOURCE_H_

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

#endif  // RADARLOVE_CORE_PIPETRIVIALSOURCE_H_
