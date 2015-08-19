// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <Compositor/PresentationEntity.h>
#include <Compositor/Primitive.h>

namespace rl {

PresentationEntity::PresentationEntity(const Entity& entity,
                                       int32_t parentIndex)
    : Entity(entity), _parentIndex(parentIndex) {
}

size_t PresentationEntity::parentIndex() const {
  return _parentIndex;
}

void PresentationEntity::render(Frame& frame) const {
  Primitive p;
  p.setContentColor(backgroundColor());
  p.setModelMatrixAndSize(modelMatrix(), bounds().size);
  p.render(frame);
}

PresentationEntity::~PresentationEntity() {
  RL_ASSERT(false);
}

}  // namespace rl
