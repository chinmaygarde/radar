// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <Interface/Entity.h>
#include <Interface/Interface.h>

namespace rl {
namespace interface {

Entity::Entity(core::Name identifier, bool notifiesInterfaceOnUpdate)
    : _identifier(identifier),
      _bounds(geom::RectZero),
      _position(geom::PointZero),
      _anchorPoint(geom::Point(0.5, 0.5)),
      _transformation(geom::MatrixIdentity),
      _backgroundColor(coordinator::ColorWhiteTransparent),
      _opacity(1.0),
      _notifiesInterfaceOnUpdate(notifiesInterfaceOnUpdate) {}

Entity::Entity(const Entity& entity)
    : _identifier(entity._identifier),
      _bounds(entity._bounds),
      _position(entity._position),
      _anchorPoint(entity._anchorPoint),
      _transformation(entity._transformation),
      _backgroundColor(entity._backgroundColor),
      _opacity(entity._opacity),
      _notifiesInterfaceOnUpdate(false) {}

void Entity::mergeProperties(const interface::Entity& entity,
                             PropertyMaskType only) {
  RL_ASSERT(_identifier == entity._identifier);

  if (only & PropertyMask::BoundsMask) {
    _bounds = entity._bounds;
  }

  if (only & PropertyMask::PositionMask) {
    _position = entity._position;
  }

  if (only & PropertyMask::AnchorPointMask) {
    _anchorPoint = entity._anchorPoint;
  }

  if (only & PropertyMask::TransformationMask) {
    _transformation = entity._transformation;
  }

  if (only & PropertyMask::BackgroundColorMask) {
    _backgroundColor = entity._backgroundColor;
  }

  if (only & PropertyMask::OpacityMask) {
    _opacity = entity._opacity;
  }
}

Entity::~Entity() {}

core::Name Entity::identifier() const {
  return _identifier;
}

geom::Rect Entity::frame() const {
  geom::Point origin(_position.x - (_bounds.size.width * _anchorPoint.x),
                     _position.y - (_bounds.size.height * _anchorPoint.y));

  return geom::Rect(origin, _bounds.size);
}

void Entity::setFrame(const geom::Rect& frame) {
  setBounds(geom::Rect(_bounds.origin, frame.size));
  setPosition(
      geom::Point(frame.origin.x + (_anchorPoint.x * frame.size.width),
                  frame.origin.y + (_anchorPoint.y * frame.size.height)));
}

const geom::Rect& Entity::bounds() const {
  return _bounds;
}

void Entity::setBounds(const geom::Rect& bounds) {
  _bounds = bounds;
  notifyInterfaceIfNecessary(Property::Bounds);
}

const geom::Point& Entity::position() const {
  return _position;
}

void Entity::setPosition(const geom::Point& position) {
  _position = position;
  notifyInterfaceIfNecessary(Property::Position);
}

const geom::Point& Entity::anchorPoint() const {
  return _anchorPoint;
}

void Entity::setAnchorPoint(const geom::Point& anchorPoint) {
  _anchorPoint = anchorPoint;
  notifyInterfaceIfNecessary(Property::AnchorPoint);
}

const geom::Matrix& Entity::transformation() const {
  return _transformation;
}

void Entity::setTransformation(const geom::Matrix& transformation) {
  _transformation = transformation;
  notifyInterfaceIfNecessary(Property::Transformation);
}

geom::Matrix Entity::modelMatrix() const {
  const geom::Point pos(_position.x - _anchorPoint.x * _bounds.size.width,
                        _position.y - _anchorPoint.y * _bounds.size.height);

  // clang-format off
  geom::Matrix translation(1.0,   0.0,   0.0, 0.0,
                     0.0,   1.0,   0.0, 0.0,
                     0.0,   0.0,   1.0, 0.0,
                     pos.x, pos.y, 0.0, 1.0);
  // clang-format on

  return translation * transformation();
}

const coordinator::Color& Entity::backgroundColor() const {
  return _backgroundColor;
}

void Entity::setBackgroundColor(const coordinator::Color& backgroundColor) {
  _backgroundColor = backgroundColor;
  notifyInterfaceIfNecessary(Property::BackgroundColor);
}

const double& Entity::opacity() const {
  return _opacity;
}

void Entity::setOpacity(double opacity) {
  _opacity = opacity;
  notifyInterfaceIfNecessary(Property::Opacity);
}

void Entity::notifyInterfaceIfNecessary(Property property,
                                        core::Name other) const {
  if (!_notifiesInterfaceOnUpdate) {
    return;
  }
  Interface::current().transaction().mark(*this, property, other);
}

enum ArchiveKey {
  Bounds,
  Position,
  AnchorPoint,
  Transformation,
  BackgroundColor,
  Opacity,
};

const core::ArchiveDef Entity::ArchiveDefinition = {
    .superClass = nullptr,
    .className = "Entity",
    .autoAssignName = false,
    .members =
        {
            ArchiveKey::Bounds,           //
            ArchiveKey::Position,         //
            ArchiveKey::AnchorPoint,      //
            ArchiveKey::Transformation,   //
            ArchiveKey::BackgroundColor,  //
            ArchiveKey::Opacity,          //
        },
};

Entity::ArchiveName Entity::archiveName() const {
  return *_identifier.handle();
}

bool Entity::serialize(core::ArchiveItem& item) const {
  auto result = true;
  result &= item.encode(ArchiveKey::Bounds, _bounds.toString());
  result &= item.encode(ArchiveKey::Position, _position.toString());
  result &= item.encode(ArchiveKey::AnchorPoint, _anchorPoint.toString());
  result &= item.encode(ArchiveKey::Transformation, _transformation.toString());
  result &=
      item.encode(ArchiveKey::BackgroundColor, _backgroundColor.toString());
  result &= item.encode(ArchiveKey::Opacity, _opacity);
  return result;
}

bool Entity::deserialize(core::ArchiveItem& item, core::Namespace* ns) {
  auto result = true;

  std::string decoded;

  result &= item.decode(ArchiveKey::Bounds, decoded);
  if (result) {
    _bounds.fromString(decoded);
  }

  result &= item.decode(ArchiveKey::Position, decoded);
  if (result) {
    _position.fromString(decoded);
  }

  result &= item.decode(ArchiveKey::AnchorPoint, decoded);
  if (result) {
    _anchorPoint.fromString(decoded);
  }

  result &= item.decode(ArchiveKey::Transformation, decoded);
  if (result) {
    _transformation.fromString(decoded);
  }

  result &= item.decode(ArchiveKey::BackgroundColor, decoded);
  if (result) {
    _backgroundColor.fromString(decoded);
  }

  result &= item.decode(ArchiveKey::Opacity, _opacity);
  return result;
}

// clang-format off
const Entity::Accessors<geom::Rect> BoundsAccessors{
  &Entity::bounds, &Entity::setBounds
};

const Entity::Accessors<geom::Point> PositionAccessors{
  &Entity::position, &Entity::setPosition
};

const Entity::Accessors<geom::Point> AnchorPointAccessors{
  &Entity::anchorPoint, &Entity::setAnchorPoint
};

const Entity::Accessors<geom::Matrix> TransformationAccessors{
  &Entity::transformation, &Entity::setTransformation
};

const Entity::Accessors<coordinator::Color> BackgroundColorAccessors{
  &Entity::backgroundColor, &Entity::setBackgroundColor
};

const Entity::Accessors<double> OpacityAccessors{
  &Entity::opacity, &Entity::setOpacity
};
// clang-format on

}  // namespace interface
}  // namespace rl
