// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef RADAR_CORE_TEMPFILEHANDLE_H_
#define RADAR_CORE_TEMPFILEHANDLE_H_

#include <Core/Macros.h>
#include <Core/FileHandle.h>

namespace rl {
namespace core {

core::URI TemporaryFileDirectory();

core::FileHandle TemporaryFileCreate(bool permanent = false);

}  // namespace core
}  // namespace rl

#endif  // RADAR_CORE_TEMPFILEHANDLE_H_
