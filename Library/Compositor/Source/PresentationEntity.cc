// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <Compositor/PresentationEntity.h>

#include "Primitive/ColoredBoxPrimitive.h"
#include "Primitive/TexturedBoxPrimitive.h"
#include "Primitive/ColoredPathPrimitive.h"
#include "Primitive/TexturedPathPrimitive.h"

#include <algorithm>

namespace rl {
namespace compositor {

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

PresentationEntity::ContentType PresentationEntity::contentType(
    double alpha) const {
  if (alpha < Primitive::AlphaThreshold) {
    return ContentType::None;
  }

  if (_contents.isValid()) {
    return ContentType::Image;
  }

  return _backgroundColor.alpha * _opacity >= Primitive::AlphaThreshold
             ? ContentType::SolidColor
             : ContentType::None;
}

PresentationEntity::PrimitiveType PresentationEntity::primitiveType() const {
  if (_path.componentCount() > 0) {
    return PrimitiveType::Path;
  }

  if (_bounds.size.width <= 0.0 || _bounds.size.height <= 0.0) {
    return PrimitiveType::None;
  } else {
    return PrimitiveType::Box;
  }

  return PrimitiveType::None;
}

std::shared_ptr<Primitive> PresentationEntity::coloredPrimitive(
    PrimitiveType type) const {
  std::shared_ptr<Primitive> primitive;

  switch (type) {
    case PrimitiveType::None:
      break;

    case PrimitiveType::Box:
      primitive = std::make_shared<ColoredBoxPrimitive>(_backgroundColor);
      break;

    case PrimitiveType::Path:
      primitive =
          std::make_shared<ColoredPathPrimitive>(_backgroundColor, _path);
      break;
  }

  if (primitive == nullptr) {
    return nullptr;
  }

  primitive->setSize(_bounds.size);
  primitive->setOpacity(_opacity);
  primitive->setModelViewMatrix(_renderedModelViewMatrix);

  return primitive;
}

std::shared_ptr<Primitive> PresentationEntity::texturedPrimitive(
    PrimitiveType type) const {
  std::shared_ptr<Primitive> primitive;

  switch (type) {
    case PrimitiveType::Box:
      primitive = std::make_shared<TexturedBoxPrimitive>(_contents);
      break;

    case PrimitiveType::Path:
      primitive = std::make_shared<TexturedPathPrimitive>(_contents, _path);
      break;

    case PrimitiveType::None:
      break;
  }

  if (primitive == nullptr) {
    return nullptr;
  }

  primitive->setSize(_bounds.size);
  primitive->setOpacity(_opacity);
  primitive->setModelViewMatrix(_renderedModelViewMatrix);

  return primitive;
}

void PresentationEntity::renderContents(FrontEndPass& frontEndPass) const {
  auto primitive = primitiveType();
  switch (contentType(_opacity)) {
    case ContentType::None:
      /*
       *  If there is no content to display, there is no sense in looking for
       *  primitives to render.
       */
      return;
    case ContentType::SolidColor:
      frontEndPass.addPrimitive(coloredPrimitive(primitive));
      break;
    case ContentType::Image:
      frontEndPass.addPrimitive(texturedPrimitive(primitive));
      break;
  }
}

}  // namespace compositor
}  // namespace rl
