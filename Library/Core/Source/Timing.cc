// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <Core/Timing.h>

namespace rl {
namespace core {
namespace clock {

core::ClockDurationSeconds LoggingClockDuration(void) {
  static core::ClockPoint point;
  static bool initialized = false;
  if (!initialized) {
    point = core::Clock::now();
    initialized = true;
  }
  return core::Clock::now() - point;
}

}  // namespace clock
}  // namespace core
}  // namespace rl
