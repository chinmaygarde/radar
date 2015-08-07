// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <Core/Timing.h>

/*
 *  TODO: Switch implementation on Linux
 */
#include <CoreFoundation/CoreFoundation.h>

namespace rl {
namespace Time {

double Current(void) {
  return CFAbsoluteTimeGetCurrent();
}

double LoggingBootTime(void) {
  static double time = -1;
  if (time == -1) {
    time = Current();
  }
  return time;
}

}  // namespace Time
}  // namespace rl
