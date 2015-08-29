// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <Compositor/PresentationEntity.h>
#include <Compositor/Primitive.h>

namespace rl {

PresentationEntity::PresentationEntity(Identifier identifier)
    : Entity(identifier) {
}

PresentationEntity::~PresentationEntity() {
}

void PresentationEntity::addChild(PresentationEntity& entity) {
  _children.push_back(&entity);
}

void PresentationEntity::removeChild(PresentationEntity& entity) {
  auto found = std::find(_children.begin(), _children.end(), &entity);
  RL_ASSERT(found != _children.end());
  _children.erase(found);
}

void PresentationEntity::render(Frame& frame) const {
  Primitive p;
  p.setContentColor(backgroundColor());
  p.setOpacity(opacity());
  p.render(frame, transformation(), bounds().size);
  frame.statistics().primitiveCount().increment();

  for (const auto& child : _children) {
    child->render(frame);
  }
}

}  // namespace rl
