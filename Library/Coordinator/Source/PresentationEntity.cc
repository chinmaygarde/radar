// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <Coordinator/PresentationEntity.h>
#include <Compositor/Primitive/ColoredBoxPrimitive.h>
#include <Compositor/Primitive/TexturedBoxPrimitive.h>

namespace rl {
namespace coordinator {

PresentationEntity::PresentationEntity(core::Name identifier)
    : Entity(identifier, nullptr) {}

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
  auto inverted = _renderedModelViewMatrix.invert();
  auto pointInWindowVector = point * inverted;
  return geom::Point{pointInWindowVector.x, pointInWindowVector.y};
}

void PresentationEntity::render(compositor::FrontEndPass& frontEndPass,
                                const geom::Matrix& viewMatrix) {
  /*
   *  Update the model view matrix for the latest render pass. This allows
   *  us to perform constant time geometry conversions between arbitrary
   *  entities.
   */

  _renderedModelViewMatrix = viewMatrix * modelMatrix();

  if (!renderContents(frontEndPass)) {
    /*
     *  The `renderContents` step explicitly told us not to bother with this
     *  subtree. Give up.
     */
    return;
  }

  for (const auto& child : _children) {
    child->render(frontEndPass, _renderedModelViewMatrix);
  }
}

bool PresentationEntity::renderContents(
    compositor::FrontEndPass& frontEndPass) const {
  /*
   *  If the size of the entity is less that zero along either dimension, there
   *  is nothing more to say about this entity.
   */
  if (_bounds.size.width <= 0.0 || _bounds.size.height <= 0.0) {
    return false;
  }

  /*
   *  Opacity must be above the threshold.
   */
  if (_opacity < compositor::Primitive::AlphaThreshold) {
    return false;
  }

  /*
   *  Step 1:
   *  Setup the primitive for the solid background color (if necessary).
   */
  if (_backgroundColor.alpha * _opacity >=
      compositor::Primitive::AlphaThreshold) {
    auto primitive = core::make_unique<compositor::ColoredBoxPrimitive>();
    primitive->setColor(_backgroundColor);
    primitive->setSize(_bounds.size);
    primitive->setOpacity(_opacity);
    primitive->setModelViewMatrix(_renderedModelViewMatrix);
    frontEndPass.addPrimitive(std::move(primitive));
  }

  /*
   *  Step 2:
   *  Setup the primitive for the contents (if necessary).
   */
  if (_contents.isValid()) {
    auto primitive = core::make_unique<compositor::TexturedBoxPrimitive>();
    primitive->setImage(_contents);
    primitive->setSize(_bounds.size);
    primitive->setOpacity(_opacity);
    primitive->setModelViewMatrix(_renderedModelViewMatrix);
    frontEndPass.addPrimitive(std::move(primitive));
  }

  return true;
}

}  // namespace coordinator
}  // namespace rl
