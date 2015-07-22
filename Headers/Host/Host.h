// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef __RADARLOVE_HOST_HOST__
#define __RADARLOVE_HOST_HOST__

#include "Core/Base.h"
#include "Core/Latch.h"

namespace rl {
class Host {
 public:
  Host() {}

  void run(Latch& readyLatch);

 private:
  DISALLOW_COPY_AND_ASSIGN(Host);
};
}

#endif /* defined(__RADARLOVE_HOST_HOST__) */
