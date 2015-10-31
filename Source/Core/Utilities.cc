// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <Core/Config.h>
#include <Core/Utilities.h>

#if RL_OS_MAC
#include <pthread/pthread.h>
#elif RL_OS_LINUX || RL_OS_NACL
#include <pthread.h>
#else
#error Unsupported Platform
#endif

namespace rl {
namespace core {

void ThreadSetName(const char* name) {
#if RL_OS_MAC
  pthread_setname_np(name);
#elif RL_OS_LINUX
  pthread_setname_np(pthread_self(), name);
#elif RL_OS_NACL
  // Unsupported on Native Client
#else
#error Unknown Platform
#endif
}

}  // namespace core
}  // namespace rl
