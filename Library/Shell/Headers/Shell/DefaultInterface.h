// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#pragma once

#include <Core/Macros.h>
#include <Interface/InterfaceDelegate.h>

namespace rl {
namespace shell {

std::unique_ptr<interface::InterfaceDelegate> CreateDefaultInterface();

}  // namespace shell
}  // namespace rl
