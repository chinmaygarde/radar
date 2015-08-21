// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef __RADARLOVE_CORE_TIME__
#define __RADARLOVE_CORE_TIME__

#include <chrono>

namespace rl {
namespace Time {

/**
 *  Get the current absolute time
 *
 *  @return the absolute time
 */
std::chrono::nanoseconds Current(void);

/**
 *  Get the absolute boot time used for logging purposes. Useless
 *  for any other purpose
 *
 *  @return the logging boot time.
 */
std::chrono::nanoseconds LoggingBootTime(void);

inline double ToSeconds(std::chrono::nanoseconds nanos) {
  return nanos.count() / 1e9;
}

}  // namespace Time
}  // namespace rl

#endif /* defined(__RADARLOVE_CORE_TIME__) */
