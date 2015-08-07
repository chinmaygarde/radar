// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef __RADAR_LOVE_CORE__
#define __RADAR_LOVE_CORE__

/*
 *  The umbrella header for the Core library
 */

#include <Core/Config.h>

/*
 *  General Utilities
 */
#include <Core/Utilities.h>
#include <Core/Macros.h>
#include <Core/Guard.h>
#include <Core/Timing.h>
#include <Core/Latch.h>

/*
 *  Channels
 */
#include <Core/ChannelProvider.h>
#include <Core/Channel.h>

/*
 *  Loopers
 */
#include <Core/Looper.h>
#include <Core/LooperObserver.h>
#include <Core/LooperSource.h>
#include <Core/WaitSet.h>

/*
 *  Messaging
 */
#include <Core/Message.h>

/*
 *  IPC
 */
#include <Core/Server.h>
#include <Core/SharedMemory.h>

#endif /* defined(__RADAR_LOVE_CORE__) */
