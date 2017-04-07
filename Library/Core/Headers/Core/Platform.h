// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#pragma once

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

#elif __fuchsia__

#define RL_OS_FUCHSIA 1

#else

#error Unsupported Platform.

#endif
