// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <Core/Config.h>
#include <Core/Timing.h>

#if RL_OS_MAC
#include <mach/mach_time.h>
#elif RL_OS_LINUX || RL_OS_NACL
#include <time.h>
#else
#error Unsupported Platform
#endif

namespace rl {
namespace Time {

#if RL_OS_MAC
static inline uint64_t TimeInNanoseconds(void) {
  static mach_timebase_info_data_t timebaseInfo = {0};
  static uint32_t ratio = 0;

  if (timebaseInfo.denom == 0) {
    (void)mach_timebase_info(&timebaseInfo);
    ratio = timebaseInfo.numer / timebaseInfo.denom;
  }

  return mach_absolute_time() * ratio;
}
#elif RL_OS_LINUX || RL_OS_NACL
static inline uint64_t TimeInNanoseconds(void) {
  struct timespec time = {0};
  clock_gettime(CLOCK_MONOTONIC, &time);
  return time.tv_sec * 1000000000 + time.tv_nsec;
}
#else
#error Unsupported Platform
#endif

std::chrono::nanoseconds Current(void) {
  return std::chrono::nanoseconds(TimeInNanoseconds());
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
