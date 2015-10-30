// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef RADARLOVE_HOST_HOST_
#define RADARLOVE_HOST_HOST_

#include <Core/Core.h>
#include <Event/TouchEventChannel.h>

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

  event::TouchEventChannel& touchEventChannel();

 private:
  event::TouchEventChannel _touchEventChannel;

  RL_DISALLOW_COPY_AND_ASSIGN(Host);
};
}  // namespace rl

#endif  // RADARLOVE_HOST_HOST_
