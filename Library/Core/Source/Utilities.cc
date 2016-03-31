// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <Core/Config.h>
#include <Core/Macros.h>
#include <Core/Utilities.h>
#include <Core/Trace.h>

#include <sys/stat.h>
#include <sys/types.h>

#if !RL_OS_WINDOWS
#include <unistd.h>
#endif  // !RL_OS_WINDOWS

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

#include <string>

namespace rl {
namespace core {

int ToUnixTimeoutMS(ClockDurationNano nano) {
  if (nano.count() >= std::numeric_limits<int>::max()) {
    return -1;
  }
  return static_cast<int>(nano.count() / 1000000);
}

#if !RL_OS_WINDOWS

void LogUnixHandleProperties(int handle) {
  RL_LOG("~~~~~~~~ Introspecting handle: %d", handle);

  struct stat statBuf = {};

  auto result = fstat(handle, &statBuf);

  if (result == -1) {
    RL_LOG("Could not stat handle: %s (%d)", strerror(errno), errno);
    return;
  }

  RL_LOG("A valid handle");

  while (true) {
    if (S_ISREG(statBuf.st_mode)) {
      RL_LOG("It is a regular file");
      break;
    }

    if (S_ISDIR(statBuf.st_mode)) {
      RL_LOG("It is a directory");
      break;
    }

    if (S_ISCHR(statBuf.st_mode)) {
      RL_LOG("It is a character device");
      break;
    }

    if (S_ISBLK(statBuf.st_mode)) {
      RL_LOG("It is a block device");
      break;
    }

    if (S_ISFIFO(statBuf.st_mode)) {
      RL_LOG("It is a FIFO (named pipe)");
      break;
    }

    if (S_ISLNK(statBuf.st_mode)) {
      RL_LOG("It is a symbolic link");
      break;
    }

    if (S_ISSOCK(statBuf.st_mode)) {
      RL_LOG("It is a socket");
      break;
    }

    RL_LOG("Not quite sure what kind of a handle this is :/");
    break;
  }

  RL_LOG("Size is: %d", static_cast<int>(statBuf.st_size));
}

#endif  // !RL_OS_WINDOWS

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
  RL_UNUSED(name);
#else
#error Unknown Platform
#endif

  instrumentation::ThreadTrace::UpdateMetadata();
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
#elif RL_OS_BSD || RL_OS_WINDOWS || RL_OS_NACL
  // No platform supported implementation
  return "";
#else
#error Unknown Platform
#endif
}

}  // namespace thread
}  // namespace core
}  // namespace rl
