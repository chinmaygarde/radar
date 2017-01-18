// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef RADAR_CORE_CONFIG_
#define RADAR_CORE_CONFIG_

#include <Core/Platform.h>

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
 *  Shared Memory
 */
#define RL_SHMEM_DISABLED 1
#define RL_SHMEM_POSIX 2
#define RL_SHMEM_ASHMEM 3

/*
 *  If cross process communication is disabled, use in-process variants of all
 *  Core primitives
 */
#if RL_DISABLE_XPC
#define RL_CHANNELS RL_CHANNELS_INPROCESS
#define RL_WAITSET RL_WAITSET_INPROCESS
#define RL_SHMEM RL_SHMEM_DISABLED
#endif

/*
 *  Detect Channels by Platform
 */
#if !defined(RL_CHANNELS)

#if RL_OS_MAC
#define RL_CHANNELS RL_CHANNELS_MACH
#elif RL_OS_RASPBERRYPI
#define RL_CHANNELS RL_CHANNELS_SOCKET
#elif RL_OS_LINUX
#define RL_CHANNELS RL_CHANNELS_SOCKET
#elif RL_OS_NACL
#define RL_CHANNELS RL_CHANNELS_INPROCESS
#elif RL_OS_WINDOWS
#define RL_CHANNELS RL_CHANNELS_INPROCESS
#elif RL_OS_BSD
#define RL_CHANNELS RL_CHANNELS_SOCKET
#elif RL_OS_FUCHSIA
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
#elif RL_OS_RASPBERRYPI
#define RL_WAITSET RL_WAITSET_EPOLL
#elif RL_OS_LINUX
#define RL_WAITSET RL_WAITSET_EPOLL
#elif RL_OS_NACL
#define RL_WAITSET RL_WAITSET_INPROCESS
#elif RL_OS_WINDOWS
#define RL_WAITSET RL_WAITSET_INPROCESS
#elif RL_OS_BSD
#define RL_WAITSET RL_WAITSET_KQUEUE
#elif RL_OS_FUCHSIA
#define RL_WAITSET RL_WAITSET_INPROCESS
#else
#error No WaitSet Implementation Found
#endif

#endif  // !defined(RL_WAITSET)

#if !defined(RL_SHMEM)

/*
 *  Detect Shared Memory by Platform
 */
#if RL_OS_MAC
#define RL_SHMEM RL_SHMEM_POSIX
#elif RL_OS_ANDROID
#define RL_SHMEM RL_SHMEM_ASHMEM
#elif RL_OS_RASPBERRYPI
#define RL_SHMEM RL_SHMEM_POSIX
#elif RL_OS_LINUX
#define RL_SHMEM RL_SHMEM_POSIX
#elif RL_OS_NACL
#define RL_SHMEM RL_SHMEM_DISABLED
#elif RL_OS_WINDOWS
#define RL_SHMEM RL_SHMEM_DISABLED
#elif RL_OS_BSD
#define RL_SHMEM RL_SHMEM_POSIX
#elif RL_OS_FUCHSIA
#define RL_SHMEM RL_SHMEM_DISABLED
#else
#error No Shared Memory Implementation Found
#endif

#endif  // !defined(RL_SHMEM)

/*
 *  In case selection by platform ends up will all in process variants (instead
 *  of the the other way around by setting RL_DISABLE_XPC), set that flag
 *  manually.
 */
#if (RL_CHANNELS == RL_CHANNELS_INPROCESS) && \
    (RL_WAITSET == RL_WAITSET_INPROCESS) && (RL_SHMEM == RL_SHMEM_DISABLED)
#define RL_DISABLE_XPC 1
#endif

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

#endif  // RADAR_CORE_CONFIG_
