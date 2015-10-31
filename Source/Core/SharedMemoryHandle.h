// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef __RADARLOVE_CORE_SHAREDMEMORYHANDLE_H__
#define __RADARLOVE_CORE_SHAREDMEMORYHANDLE_H__

#include <Core/SharedMemory.h>

namespace rl {
namespace core {

SharedMemory::Handle SharedMemoryHandleCreate();

}  // namespace core
}  // namespace rl

#endif  // __RADARLOVE_CORE_SHAREDMEMORYHANDLE_H__
