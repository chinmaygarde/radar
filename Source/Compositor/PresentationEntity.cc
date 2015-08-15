// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <Compositor/PresentationEntity.h>

namespace rl {

PresentationEntity::PresentationEntity(const Entity& entity) : Entity(entity) {
}

PresentationEntity::~PresentationEntity() {
  RL_ASSERT(false);
}

}  // namespace rl
