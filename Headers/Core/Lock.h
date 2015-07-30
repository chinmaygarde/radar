// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef __RADARLOVE__LOCK__
#define __RADARLOVE__LOCK__

#include <pthread.h>
#include "Core/Base.h"

namespace rl {

class Lock {
 public:
  Lock() { pthread_mutex_init(&_mutex, NULL); }

  ~Lock() { pthread_mutex_destroy(&_mutex); }

  void lock() const { pthread_mutex_lock(&_mutex); }

  void unlock() const { pthread_mutex_unlock(&_mutex); }

 private:
  mutable pthread_mutex_t _mutex;
};
}

#endif /* defined(__RADARLOVE__LOCK__) */
