// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "Core/Timing.h"

/*
 *  TODO: Switch implementation on Linux
 */
#include <CoreFoundation/CoreFoundation.h>

namespace rl {
namespace Time {

double Current(void) {
  return CFAbsoluteTimeGetCurrent();
}

}  // namespace Time
}  // namespace rl
