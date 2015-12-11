// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <Interface/Entity.h>
#include <Interface/Interface.h>

namespace rl {
namespace interface {

static Entity::Identifier LastEntityIdentifier = 0;

Entity::Entity(bool notifiesInterfaceOnUpdate)
    : _identifier(++LastEntityIdentifier),
      _bounds(geom::RectZero),
      _position(geom::PointZero),
      _anchorPoint(geom::Point(0.5, 0.5)),
      _transformation(geom::MatrixIdentity),
      _backgroundColor(coordinator::ColorWhiteTransparent),
      _opacity(1.0),
      _notifiesInterfaceOnUpdate(notifiesInterfaceOnUpdate) {
  notifyInterfaceIfNecessary(Created);
}

Entity::Entity(Identifier identifier)
    : _identifier(identifier),
      _bounds(geom::RectZero),
      _position(geom::PointZero),
      _anchorPoint(geom::Point(0.5, 0.5)),
      _transformation(geom::MatrixIdentity),
      _backgroundColor(coordinator::ColorWhiteTransparent),
      _opacity(1.0),
      _notifiesInterfaceOnUpdate(false) {}

Entity::Entity(const Entity& entity)
    : _identifier(entity._identifier),
      _bounds(entity._bounds),
      _position(entity._position),
      _anchorPoint(entity._anchorPoint),
      _transformation(entity._transformation),
      _backgroundColor(entity._backgroundColor),
      _opacity(entity._opacity),
      _notifiesInterfaceOnUpdate(false) {}

void Entity::merge(const interface::Entity& entity) {
  RL_ASSERT(_identifier == entity._identifier);

  _bounds = entity._bounds;
  _position = entity._position;
  _anchorPoint = entity._anchorPoint;
  _transformation = entity._transformation;
  _backgroundColor = entity._backgroundColor;
  _opacity = entity._opacity;
}

Entity::~Entity() {
  notifyInterfaceIfNecessary(Destroyed);
}

uint64_t Entity::identifier() const {
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
  notifyInterfaceIfNecessary(Bounds);
}

const geom::Point& Entity::position() const {
  return _position;
}

void Entity::setPosition(const geom::Point& position) {
  _position = position;
  notifyInterfaceIfNecessary(Position);
}

const geom::Point& Entity::anchorPoint() const {
  return _anchorPoint;
}

void Entity::setAnchorPoint(const geom::Point& anchorPoint) {
  _anchorPoint = anchorPoint;
  notifyInterfaceIfNecessary(AnchorPoint);
}

const geom::Matrix& Entity::transformation() const {
  return _transformation;
}

void Entity::setTransformation(const geom::Matrix& transformation) {
  _transformation = transformation;
  notifyInterfaceIfNecessary(Transformation);
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
  notifyInterfaceIfNecessary(BackgroundColor);
}

const double& Entity::opacity() const {
  return _opacity;
}

void Entity::setOpacity(double opacity) {
  _opacity = opacity;
  notifyInterfaceIfNecessary(Opacity);
}

void Entity::notifyInterfaceIfNecessary(Property property,
                                        Identifier other) const {
  if (!_notifiesInterfaceOnUpdate) {
    return;
  }
  Interface::current().transaction().mark(*this, property, other);
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
