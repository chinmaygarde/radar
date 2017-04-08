/*
 *  This source file is part of the Radar project.
 *  Licensed under the MIT License. See LICENSE file for details.
 */

#include "WaitSetProvider.h"
#include <Core/EventLoopSource.h>

namespace rl {
namespace core {

void WaitSetProvider::updateSource(WaitSet& waitset,
                                   EventLoopSource& source,
                                   bool addedOrRemoved) {
  source.updateInWaitSet(waitset, addedOrRemoved);
}

WaitSetProvider::~WaitSetProvider() {}

}  // namespace core
}  // namespace rl
