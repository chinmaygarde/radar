// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <Core/Config.h>
#include <Core/Macros.h>
#include <Core/Utilities.h>
#include <Core/Trace.h>

#include <sys/stat.h>
#include <sys/types.h>

#if !RL_OS_WINDOWS
#include <unistd.h>
#endif  // !RL_OS_WINDOWS

#include <string>

namespace rl {
namespace core {

int ToUnixTimeoutMS(ClockDurationNano nano) {
  if (nano.count() >= std::numeric_limits<int>::max()) {
    return -1;
  }
  return static_cast<int>(nano.count() / 1000000);
}

}  // namespace core
}  // namespace rl
