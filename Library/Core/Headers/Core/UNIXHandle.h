/*
 *  This source file is part of the Radar project.
 *  Licensed under the MIT License. See LICENSE file for details.
 */

#pragma once

#include <Core/Handle.h>
#include <unistd.h>

namespace rl {
namespace core {

struct UNIXHandleTraits {
  static int NullHandle() { return -1; };

  static bool IsValid(int handle) { return handle >= 0; }

  static bool CollectHandle(int handle) {
    RL_TEMP_FAILURE_RETRY(::close(handle));
    return true;
  }
};

using UNIXHandle = Handle<int, UNIXHandleTraits>;

}  // namespace core
}  // namespace rl
