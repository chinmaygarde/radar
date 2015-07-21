// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef __RADARLOVE__AUTOLOCK__
#define __RADARLOVE__AUTOLOCK__

#include "Lock.h"

namespace rl {
class AutoLock {
 public:
  AutoLock(const Lock& lock) : _lock(lock) { _lock.lock(); }

  ~AutoLock() { _lock.unlock(); }

 private:
  const Lock& _lock;

  DISALLOW_COPY_AND_ASSIGN(AutoLock);
};

} /* namespace rl */

#endif /* defined(__RADARLOVE__AUTOLOCK__) */
