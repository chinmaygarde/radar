// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <Compositor/PresentationEntity.h>

namespace rl {

PresentationEntity::PresentationEntity(const Entity& entity,
                                       int32_t parentIndex)
    : Entity(entity), _parentIndex(parentIndex) {
}

size_t PresentationEntity::parentIndex() const {
  return _parentIndex;
}

PresentationEntity::~PresentationEntity() {
  RL_ASSERT(false);
}

}  // namespace rl
