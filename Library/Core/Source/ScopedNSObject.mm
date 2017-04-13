/*
 *  This source file is part of the Radar project.
 *  Licensed under the MIT License. See LICENSE file for details.
 */

#include <Core/Platform.h>
#include <Core/ScopedNSObject.h>

#if RL_OS_MAC

#include <Foundation/NSObject.h>

namespace rl {
namespace core {

void ObjCRetain(void* object) {
  [reinterpret_cast<id>(object) retain];
}

void ObjCRelease(void* object) {
  [reinterpret_cast<id>(object) release];
}

}  // namespace core
}  // namespace rl

#endif  // RL_OS_MAC
