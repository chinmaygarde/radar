// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <Core/Config.h>

#if RL_CHANNELS == RL_CHANNELS_MACH

#include "MachBootstrapServerProvider.h"

namespace rl {
namespace core {

MachBootstrapServerProvider::MachBootstrapServerProvider() = default;

MachBootstrapServerProvider::~MachBootstrapServerProvider() = default;

/*
 *  Nothing much to do here since we are piggy-backing off of the platform
 *  bootstrap server.
 */

}  // namespace core
}  // namespace rl

#endif  // RL_CHANNELS == RL_CHANNELS_SOCKET
