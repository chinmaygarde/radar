// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef __RADARLOVE_HOST_HOST__
#define __RADARLOVE_HOST_HOST__

#include <Core/Core.h>

namespace rl {
class Host {
 public:
  explicit Host();

  /**
   *  Run the host access subsystem
   *
   *  @param readyLatch the latch to count down on when the subsystem has run
   */
  void run(Latch& readyLatch);

  /**
   *  Gracefully shutdown the subsystem
   *
   *  @param shutdownLatch the latch to cound down on when the subsystem shuts
   *         down
   */
  void shutdown(Latch& shutdownLatch);

 private:
  DISALLOW_COPY_AND_ASSIGN(Host);
};
}

#endif /* defined(__RADARLOVE_HOST_HOST__) */
