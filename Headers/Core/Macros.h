// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef __RADARLOVE_CORE_MACROS__
#define __RADARLOVE_CORE_MACROS__

#include <Core/Config.h>
#include <Core/Utilities.h>
#include <Core/Timing.h>
#include <string.h>
#include <errno.h>
#include <stdio.h>
#include <assert.h>

#pragma mark - Assertions

#define RL_ASSERT assert

#pragma mark - Logging

#define _RL_FILE_LAST_COMPONENT \
  (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : __FILE__)

#define _RL_LOG_FMT "%.3fs %s:%d: "
#define _RL_LOG_ARG                                                       \
  rl::Time::ToSeconds(rl::Time::Current() - rl::Time::LoggingBootTime()), \
      _RL_FILE_LAST_COMPONENT, __LINE__

#define RL_LOG(message, ...) \
  printf(_RL_LOG_FMT message "\n", _RL_LOG_ARG, ##__VA_ARGS__);
#define RL_LOG_ERRNO() RL_LOG("%s (%d)", strerror(errno), errno)
#define RL_LOG_HERE RL_LOG("%s", __FUNCTION__)

#pragma mark - Error Checking

#define _RL_CHECK_EXPECT(call, expected)    \
  {                                         \
    if ((call) != (expected)) {             \
      RL_LOG_ERRNO();                       \
      RL_ASSERT(0 && "Error in UNIX call"); \
    }                                       \
  }

#define RL_CHECK(call) _RL_CHECK_EXPECT(call, 0)

#pragma mark - POSIX Retry

/**
 *  Retries operation on `EINTR`. Just like `TEMP_FAILURE_RETRY` but available
 *  with non-GNU headers
 */
#define RL_TEMP_FAILURE_RETRY(exp)         \
  ({                                       \
    __typeof__(exp) _rc;                   \
    do {                                   \
      _rc = (exp);                         \
    } while (_rc == -1 && errno == EINTR); \
    _rc;                                   \
  })

#define RL_TEMP_FAILURE_RETRY_AND_CHECK(exp) \
  ({                                         \
    __typeof__(exp) _rc;                     \
    do {                                     \
      _rc = (exp);                           \
    } while (_rc == -1 && errno == EINTR);   \
    _RL_CHECK_EXPECT(_rc, 0);                \
    _rc;                                     \
  })

#define RL_DISALLOW_COPY(TypeName) TypeName(TypeName&) = delete;

#define RL_DISALLOW_ASSIGN(TypeName) void operator=(TypeName) = delete;

#define RL_DISALLOW_COPY_AND_ASSIGN(TypeName) \
  TypeName(TypeName&) = delete;               \
  void operator=(TypeName) = delete;

#endif /* defined(__RADARLOVE_CORE_MACROS__) */
