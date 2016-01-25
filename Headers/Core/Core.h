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
#include <Core/Allocation.h>
#include <Core/Guard.h>
#include <Core/Latch.h>
#include <Core/Macros.h>
#include <Core/Namespace.h>
#include <Core/Timing.h>
#include <Core/Utilities.h>

/*
 *  Bootstrap Server
 */
#include <Core/BootstrapServer.h>

/*
 *  Channels
 */
#include <Core/Channel.h>
#include <Core/Protocol.h>

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
#include <Core/MessageSerializable.h>

/*
 *  IPC
 */
#include <Core/SharedMemory.h>

/*
 *  Work Queues
 */
#include <Core/WorkQueue.h>

#endif  // RADARLOVE_CORE_
