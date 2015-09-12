// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef __RADARLOVE_CORE_CONFIG__
#define __RADARLOVE_CORE_CONFIG__

#if __APPLE__
#define RL_OS_MAC 1
#elif __native_client__
#define RL_OS_NACL 1
#elif __linux__
#define RL_OS_LINUX 1
#else
#error Unsupported Platform
#endif

#endif /* defined(__RADARLOVE_CORE_CONFIG__) */
