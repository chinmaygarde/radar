// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef RADARLOVE_CORE_SHAREDMEMORYHANDLE_H_
#define RADARLOVE_CORE_SHAREDMEMORYHANDLE_H_

#include <Core/SharedMemory.h>

namespace rl {
namespace core {

SharedMemory::Handle SharedMemoryHandleCreate(size_t size);

}  // namespace core
}  // namespace rl

#endif  // RADARLOVE_CORE_SHAREDMEMORYHANDLE_H_