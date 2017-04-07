// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#pragma once

#include <Core/SharedMemory.h>

namespace rl {
namespace core {

SharedMemory::Handle SharedMemoryHandleCreate(size_t size);

}  // namespace core
}  // namespace rl
