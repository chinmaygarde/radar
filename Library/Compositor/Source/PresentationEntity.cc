// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <Compositor/PresentationEntity.h>
#include <algorithm>
#include "PrimitivesCache.h"

namespace rl {
namespace compositor {

PresentationEntity::PresentationEntity(core::Name identifier)
    : Entity(identifier, nullptr),
      _primitivesCache(core::make_unique<PrimitivesCache>()) {}

PresentationEntity::~PresentationEntity() = default;

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

void PresentationEntity::render(FrontEndPass& frontEndPass,
                                const geom::Matrix& viewMatrix) {
  /*
   *  Update the model view matrix for the latest render pass. This allows
   *  us to perform constant time geometry conversions between arbitrary
   *  entities.
   */

  _renderedModelViewMatrix = viewMatrix * modelMatrix();

  renderContents(frontEndPass);

  for (const auto& child : _children) {
    child->render(frontEndPass, _renderedModelViewMatrix);
  }
}

void PresentationEntity::renderContents(FrontEndPass& frontEndPass) const {
  // Decide the content type.
  PrimitivesCache::ContentType contentType = PrimitivesCache::ContentType::None;
  if (_opacity < Primitive::AlphaThreshold) {
    contentType = PrimitivesCache::ContentType::None;
  } else if (_contents.isValid()) {
    contentType = PrimitivesCache::ContentType::Image;
  } else {
    contentType = _backgroundColor.alpha * _opacity >= Primitive::AlphaThreshold
                      ? PrimitivesCache::ContentType::SolidColor
                      : PrimitivesCache::ContentType::None;
  }

  // Decide the primitive type.
  PrimitivesCache::PrimitiveType primitiveType =
      PrimitivesCache::PrimitiveType::None;
  if (_path.componentCount() > 0) {
    primitiveType = PrimitivesCache::PrimitiveType::Path;
  } else if (_bounds.size.width <= 0.0 || _bounds.size.height <= 0.0) {
    primitiveType = PrimitivesCache::PrimitiveType::None;
  } else {
    primitiveType = PrimitivesCache::PrimitiveType::Box;
  }

  if (auto primitive =
          _primitivesCache->acquire(*this, contentType, primitiveType)) {
    primitive->setSize(_bounds.size);
    primitive->setOpacity(_opacity);
    primitive->setModelViewMatrix(_renderedModelViewMatrix);
    frontEndPass.addPrimitive(std::move(primitive));
  }
}

}  // namespace compositor
}  // namespace rl
