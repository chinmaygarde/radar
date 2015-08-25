// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <Compositor/PresentationEntity.h>
#include <Compositor/Primitive.h>

namespace rl {

PresentationEntity::PresentationEntity(const Entity& entity) : Entity(entity) {
}

PresentationEntity::PresentationEntity(const PresentationEntity& entity)
    : Entity(entity) {
}

PresentationEntity::~PresentationEntity() {
}

void PresentationEntity::render(Frame& frame) const {
  Primitive p;
  p.setContentColor(backgroundColor());
  p.setOpacity(opacity());
  p.render(frame, transformation(), bounds().size);
  frame.statistics().primitiveCount().increment();
}

}  // namespace rl
