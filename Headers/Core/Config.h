// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef __RADARLOVE_CORE_CONFIG__
#define __RADARLOVE_CORE_CONFIG__

/*
 *  Platform
 */

#if __APPLE__
#define RL_OS_MAC 1
#elif __linux__
#define RL_OS_LINUX 1
#elif __native_client__
#define RL_OS_NACL 1
#else
#error Unsupported Platform
#endif

/*
 *  Channels
 */

#define RL_CHANNELS_MACH 1
#define RL_CHANNELS_SOCKET 2
#define RL_CHANNELS_INPROCESS 3

/*
 *  WaitSet
 */

#define RL_WAITSET_KQUEUE 1
#define RL_WAITSET_EPOLL 2
#define RL_WAITSET_INPROCESS 3

/*
 *  Add overrides here...
 */
// #define RL_CHANNELS RL_CHANNELS_INPROCESS
// #define RL_WAITSET RL_WAITSET_INPROCESS

/*
 *  Detect Channels by Platform
 */

#if !defined(RL_CHANNELS)

#if RL_OS_MAC
#define RL_CHANNELS RL_CHANNELS_MACH
#elif RL_OS_LINUX
#define RL_CHANNELS RL_CHANNELS_SOCKET
#elif RL_OS_NACL
#define RL_CHANNELS RL_CHANNELS_INPROCESS
#else
#error No Channel Implementation Found
#endif

#endif  // !defined(RL_CHANNELS)

/*
 *  Detect Waitset by Platform
 */

#if !defined(RL_WAITSET)

#if RL_OS_MAC
#define RL_WAITSET RL_WAITSET_KQUEUE
#elif RL_OS_LINUX
#define RL_WAITSET RL_WAITSET_EPOLL
#elif RL_OS_NACL
#define RL_WAITSET RL_WAITSET_INPROCESS
#else
#error No WaitSet Implementation Found
#endif

#endif  // !defined(RL_WAITSET)

/*
 *  Assertions against bad configurations
 */
#if RL_CHANNELS == RL_CHANNELS_MACH
#if !(RL_WAITSET_KQUEUE) || !(RL_OS_MAC)
#error Can only use mach channels using kqueue waitsets on Mac
#endif
#endif

#if RL_WAITSET == RL_WAITSET_EPOLL && !(RL_OS_LINUX)
#error Can only use epoll on linux
#endif

#if RL_CHANNELS == RL_CHANNELS_INPROCESS && RL_WAITSET != RL_WAITSET_INPROCESS
#error in-process channels require in-process waitsets
#endif

#if RL_WAITSET == RL_WAITSET_INPROCESS && RL_CHANNELS != RL_CHANNELS_INPROCESS
#error in-process waitsets require in-process channels
#endif

#endif  // __RADARLOVE_CORE_CONFIG__
