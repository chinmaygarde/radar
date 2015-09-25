// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <Core/Config.h>
#include <Core/Timing.h>

namespace rl {
namespace clock {

ClockPoint LoggingBootTime(void) {
  static ClockPoint point;
  static bool initialized = false;
  if (!initialized) {
    point = Clock::now();
    initialized = true;
  }
  return point;
}

}  // namespace clock
}  // namespace rl
