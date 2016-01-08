// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <Coordinator/PresentationEntity.h>
#include <Coordinator/Primitive.h>

namespace rl {
namespace coordinator {

PresentationEntity::PresentationEntity(core::Name identifier)
    : Entity(identifier, false), _lastModelViewMatrix(geom::MatrixIdentity) {}

PresentationEntity::~PresentationEntity() {}

void PresentationEntity::addChild(Borrowed entity) {
  _children.push_back(entity);
}

void PresentationEntity::removeChild(Borrowed entity) {
  auto found = std::find(_children.begin(), _children.end(), entity);
  RL_ASSERT(found != _children.end());
  _children.erase(found);
}

bool PresentationEntity::isWindowPointInside(const geom::Point& point) const {
  return bounds().contains(convertPointFromWindow(point));
}

geom::Point PresentationEntity::convertPointFromWindow(
    const geom::Point& point) const {
  auto inverted = _lastModelViewMatrix.invert();
  auto pointInWindowVector = point * inverted;
  return geom::Point{pointInWindowVector.a, pointInWindowVector.b};
}

void PresentationEntity::render(Frame& frame, const geom::Matrix& viewMatrix) {
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

}  // namespace coordinator
}  // namespace rl
