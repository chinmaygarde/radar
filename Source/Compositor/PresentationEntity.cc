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

void PresentationEntity::addChild(Borrowed entity) {
  _children.push_back(entity);
}

void PresentationEntity::removeChild(Borrowed entity) {
  auto found = std::find(_children.begin(), _children.end(), entity);
  RL_ASSERT(found != _children.end());
  _children.erase(found);
}

void PresentationEntity::render(Frame& frame, const Matrix& viewMatrix) const {
  frame.statistics().primitiveCount().increment();

  Matrix modelViewMatrix = viewMatrix * modelMatrix();

  Primitive p;
  p.setContentColor(backgroundColor());
  p.setOpacity(opacity());
  p.render(frame, modelViewMatrix, bounds().size);

  for (const auto& child : _children) {
    child->render(frame, modelViewMatrix);
  }
}

}  // namespace rl
