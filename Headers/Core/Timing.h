// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef __RADARLOVE_CORE_TIME__
#define __RADARLOVE_CORE_TIME__

namespace rl {
namespace Time {

/**
 *  Get the current absolute time (in seconds)
 *
 *  @return the absolute time
 */
double Current(void);

/**
 *  Get the absolute boot time used for logging purposes (in seconds). Useless
 *  for any other purpose
 *
 *  @return the logging boot time.
 */
double LoggingBootTime(void);

}  // namespace Time
}  // namespace rl

#endif /* defined(__RADARLOVE_CORE_TIME__) */
