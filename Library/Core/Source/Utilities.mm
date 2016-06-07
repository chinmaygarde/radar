// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <Core/Config.h>
#include <Core/Utilities.h>

#if RL_OS_MAC
#include <Foundation/Foundation.h>
#endif

namespace rl {
namespace core {

int ToUnixTimeoutMS(ClockDurationNano nano) {
  if (nano.count() >= std::numeric_limits<int>::max()) {
    return -1;
  }
  return static_cast<int>(nano.count() / 1000000);
}

URI GetExecutablePath() {
#if RL_OS_MAC
  return URI{[[NSBundle mainBundle] executablePath].UTF8String};
#else
#error Unknown Platform
#endif
}

}  // namespace core
}  // namespace rl
