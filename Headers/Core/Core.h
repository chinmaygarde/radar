// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef RADARLOVE_CORE_
#define RADARLOVE_CORE_

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
#include <Core/Channel.h>

/*
 *  Event Loops
 */
#include <Core/EventLoop.h>
#include <Core/EventLoopObserver.h>
#include <Core/EventLoopSource.h>
#include <Core/WaitSet.h>

/*
 *  Messaging
 */
#include <Core/Message.h>
#include <Core/Serializable.h>

/*
 *  IPC
 */
#include <Core/SharedMemory.h>

/*
 *  Work Queues
 */
#include <Core/WorkQueue.h>

#endif  // RADARLOVE_CORE_
