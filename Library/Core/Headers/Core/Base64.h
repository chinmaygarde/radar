// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef RADAR_CORE_BASE64_H_
#define RADAR_CORE_BASE64_H_

#include <Core/Allocation.h>
#include <Core/Macros.h>

namespace rl {
namespace core {

Allocation Base64Decode(const uint8_t* data);

}  // namespace core
}  // namespace rl

#endif  // RADAR_CORE_BASE64_H_
