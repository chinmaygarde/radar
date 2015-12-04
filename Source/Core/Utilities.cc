// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <Core/Config.h>
#include <Core/Utilities.h>
#include <Core/Macros.h>
#include <string>

#if RL_OS_MAC
#include <pthread/pthread.h>
#elif RL_OS_LINUX
#include <pthread.h>
#elif RL_OS_BSD
#include <pthread.h>
#include <pthread_np.h>
#elif RL_OS_WINDOWS || RL_OS_NACL
// No platform supported implementation
#else
#error Unsupported Platform
#endif

namespace rl {
namespace core {
namespace thread {

void SetName(const char* name) {
#if RL_OS_MAC
  pthread_setname_np(name);
#elif RL_OS_LINUX
  pthread_setname_np(pthread_self(), name);
#elif RL_OS_BSD
  pthread_set_name_np(pthread_self(), name);
#elif RL_OS_WINDOWS || RL_OS_NACL
// No platform supported implementation
#else
#error Unknown Platform
#endif
}

std::string GetName() {
#if RL_OS_MAC
  char name[24] = {0};
  RL_CHECK(pthread_getname_np(pthread_self(), name, sizeof(name)));
  return name;
#elif RL_OS_LINUX
  char name[24] = {0};
  RL_CHECK(pthread_getname_np(pthread_self(), name, sizeof(name)));
  return name;
#else
#error Unknown Platform
#endif
}

}  // namespace thread
}  // namespace core
}  // namespace rl
