/*
 *  This source file is part of the Radar project.
 *  Licensed under the MIT License. See LICENSE file for details.
 */

#pragma once

#include <Core/EventLoopSource.h>
#include <Core/Macros.h>
#include <Core/Timing.h>
#include <Core/WaitSet.h>

namespace rl {
namespace core {

class WaitSetProvider {
 public:
  virtual WaitSet::Result wait(ClockDurationNano timeout) = 0;

  virtual WaitSet::Handle handle() const = 0;

  virtual void updateSource(WaitSet& waitset,
                            EventLoopSource& source,
                            bool addedOrRemoved);

  virtual ~WaitSetProvider();
};

}  // namespace core
}  // namespace rl
