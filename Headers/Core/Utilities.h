// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef __RADARLOVE__UTILITIES__
#define __RADARLOVE__UTILITIES__

#include "Config.h"
#include <string.h>
#include <errno.h>
#include <stdio.h>
#include <assert.h>

#pragma mark - Assertions

#define RL_ASSERT assert

#pragma mark - Logging

#if RL_ENABLE_LOGS

#define RL_FILE_LAST_COMPONENT \
  (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : __FILE__)

#define RL_LOG_FMT "Radar (in '%s' @ %s:%d): "
#define RL_LOG_ARG __FUNCTION__, RL_FILE_LAST_COMPONENT, __LINE__

#define RL_LOG(message, ...) \
  printf(RL_LOG_FMT message "\n", RL_LOG_ARG, ##__VA_ARGS__)
#define RL_LOG_ERRNO() RL_LOG("%s (%d)", strerror(errno), errno)

#else

#define RL_LOG(message, ...)
#define RL_LOG_ERRNO()

#endif

#pragma mark - Error Checking

#define RL_CHECK_EXPECT(call, expected)     \
  {                                         \
    if ((call) != (expected)) {             \
      RL_LOG_ERRNO();                       \
      RL_ASSERT(0 && "Error in UNIX call"); \
    }                                       \
  }

#define RL_CHECK(call) RL_CHECK_EXPECT(call, 0)

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
    RL_CHECK_EXPECT(_rc, 0);                 \
    _rc;                                     \
  })

#endif /* defined(__RADARLOVE__UTILITIES__) */
