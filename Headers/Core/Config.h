// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef RADARLOVE_CORE_CONFIG_
#define RADARLOVE_CORE_CONFIG_

/*
 *  Platform
 */

#if __APPLE__

#define RL_OS_MAC 1

#elif __linux__

#define RL_OS_LINUX 1

#if __ANDROID__
/*
 *  Android is a Linux subtype
 */
#define RL_OS_ANDROID 1
#endif

#if __raspberrypi__
/*
 *  RaspberryPi is considered a linux subtype (for Raspbian). Unlike other
 *  targets. The define this option is swithed on is specified manually in the
 *  build manifest.
 */
#define RL_OS_RASPBERRYPI 1
#endif

#elif __native_client__

#define RL_OS_NACL 1

#elif _WIN32

#define RL_OS_WINDOWS 1

#elif __FreeBSD__

#define RL_OS_BSD 1

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
 *  Shared Memory
 */
#define RL_SHMEM_DISABLED 1
#define RL_SHMEM_POSIX 2
#define RL_SHMEM_ASHMEM 3

/*
 *  Add overrides here...
 */
// #define RL_CHANNELS RL_CHANNELS_INPROCESS
// #define RL_WAITSET RL_WAITSET_INPROCESS
// #define RL_SHMEM RL_SHMEM_POSIX

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
#else
#error No Shared Memory Implementation Found
#endif

#endif  // !defined(RL_SHMEM)

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

#endif  // RADARLOVE_CORE_CONFIG_
