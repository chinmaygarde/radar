// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <Core/Timing.h>

/*
 *  TODO: Switch implementation on Linux
 */
#include <mach/mach_time.h>

namespace rl {
namespace Time {

static inline uint64_t MachTimeInNanoseconds(void) {
  static mach_timebase_info_data_t timebaseInfo = {0};
  static uint32_t ratio = 0;

  if (timebaseInfo.denom == 0) {
    (void)mach_timebase_info(&timebaseInfo);
    ratio = timebaseInfo.numer / timebaseInfo.denom;
  }

  return mach_absolute_time() * ratio;
}

std::chrono::nanoseconds Current(void) {
  return std::chrono::nanoseconds(MachTimeInNanoseconds());
}

std::chrono::nanoseconds LoggingBootTime(void) {
  static std::chrono::nanoseconds time(0);
  if (time.count() == 0) {
    time = Current();
  }
  return time;
}

}  // namespace Time
}  // namespace rl
