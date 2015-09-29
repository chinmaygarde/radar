// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <Compositor/PresentationEntity.h>
#include <Compositor/Primitive.h>

namespace rl {

static const PresentationEntity::PresentationOrder PresentationOrderNone = 0;

PresentationEntity::PresentationEntity(Identifier identifier)
    : Entity(identifier),
      _presentationOrder(PresentationOrderNone),
      _lastModelViewMatrix(MatrixIdentity) {
}

PresentationEntity::~PresentationEntity() {
}

void PresentationEntity::addChild(Borrowed entity) {
  _children.push_back(entity);
  entity->_presentationOrder = _presentationOrder + 1;
}

void PresentationEntity::removeChild(Borrowed entity) {
  auto found = std::find(_children.begin(), _children.end(), entity);
  RL_ASSERT(found != _children.end());
  _children.erase(found);
  entity->_presentationOrder = PresentationOrderNone;
}

bool PresentationEntity::isPointInside(const Point& point) const {
  RL_ASSERT(false);
  return false;
}

void PresentationEntity::render(Frame& frame, const Matrix& viewMatrix) {
  frame.statistics().primitiveCount().increment();

  /*
   *  This is incorrect. This method needs to be const.
   */
  _lastModelViewMatrix = viewMatrix * modelMatrix();

  Primitive p;
  p.setContentColor(backgroundColor());
  p.setOpacity(opacity());
  p.render(frame, _lastModelViewMatrix, bounds().size);

  for (const auto& child : _children) {
    child->render(frame, _lastModelViewMatrix);
  }
}

}  // namespace rl
