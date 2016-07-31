// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "Config.h"

#if RL_CHANNELS == RL_CHANNELS_INPROCESS

#include "InProcessAttachment.h"

namespace rl {
namespace core {

InProcessAttachment::InProcessAttachment() {}

InProcessAttachment::~InProcessAttachment() {}

}  // namespace core
}  // namespace rl

#endif  // RL_CHANNELS == RL_CHANNELS_INPROCESS
