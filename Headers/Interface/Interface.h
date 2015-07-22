// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef __RADARLOVE_INTERFACE_INTERFACE__
#define __RADARLOVE_INTERFACE_INTERFACE__

#include "Core/Base.h"
#include "Core/Looper.h"
#include "Core/Lock.h"
#include "Core/Latch.h"

namespace rl {
class Interface {
 public:
  Interface() : _looper(nullptr), _lock() {}
  void run(Latch& readyLatch);

  bool isRunning() { return _looper != nullptr; }

 private:
  Looper* _looper;
  Lock _lock;

  DISALLOW_COPY_AND_ASSIGN(Interface);
};
}

#endif /* defined(__RADARLOVE_INTERFACE_INTERFACE__) */
