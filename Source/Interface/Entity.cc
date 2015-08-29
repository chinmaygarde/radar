// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <Interface/Entity.h>
#include <Interface/Interface.h>

namespace rl {

static Entity::Identifier LastEntityIdentifier = 0;

Entity::Entity(bool notifiesInterfaceOnUpdate)
    : _identifier(++LastEntityIdentifier),
      _bounds(RectZero),
      _position(PointZero),
      _anchorPoint(Point(0.5, 0.5)),
      _transformation(MatrixIdentity),
      _backgroundColor(ColorWhiteTransparent),
      _opacity(1.0),
      _notifiesInterfaceOnUpdate(notifiesInterfaceOnUpdate) {
  notifyInterfaceIfNecessary(Created);
}

Entity::Entity(Identifier identifier)
    : _identifier(identifier),
      _bounds(RectZero),
      _position(PointZero),
      _anchorPoint(Point(0.5, 0.5)),
      _transformation(MatrixIdentity),
      _backgroundColor(ColorWhiteTransparent),
      _opacity(1.0),
      _notifiesInterfaceOnUpdate(false) {
}

Entity::Entity(const Entity& entity)
    : _identifier(entity._identifier),
      _bounds(entity._bounds),
      _position(entity._position),
      _anchorPoint(entity._anchorPoint),
      _transformation(entity._transformation),
      _backgroundColor(entity._backgroundColor),
      _opacity(entity._opacity),
      _notifiesInterfaceOnUpdate(false) {
}

void Entity::merge(const rl::Entity& entity) {
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

Rect Entity::frame() const {
  Point origin(_position.x - (_bounds.size.width * _anchorPoint.x),
               _position.y - (_bounds.size.height * _anchorPoint.y));

  return Rect(origin, _bounds.size);
}

void Entity::setFrame(const Rect& frame) {
  setBounds(Rect(_bounds.origin, frame.size));
  setPosition(Point(frame.origin.x + (_anchorPoint.x * frame.size.width),
                    frame.origin.y + (_anchorPoint.y * frame.size.height)));
}

const Rect& Entity::bounds() const {
  return _bounds;
}

void Entity::setBounds(const Rect& bounds) {
  _bounds = bounds;
  notifyInterfaceIfNecessary(Bounds);
}

const Point& Entity::position() const {
  return _position;
}

void Entity::setPosition(const Point& position) {
  _position = position;
  notifyInterfaceIfNecessary(Position);
}

const Point& Entity::anchorPoint() const {
  return _anchorPoint;
}

void Entity::setAnchorPoint(const Point& anchorPoint) {
  _anchorPoint = anchorPoint;
  notifyInterfaceIfNecessary(AnchorPoint);
}

const Matrix& Entity::transformation() const {
  return _transformation;
}

void Entity::setTransformation(const Matrix& transformation) {
  _transformation = transformation;
  notifyInterfaceIfNecessary(Transformation);
}

Matrix Entity::modelMatrix() const {
  const Point pos(_position.x - _anchorPoint.x * _bounds.size.width,
                  _position.y - _anchorPoint.y * _bounds.size.height);

  // clang-format off
  Matrix translation(1.0,   0.0,   0.0, 0.0,
                     0.0,   1.0,   0.0, 0.0,
                     0.0,   0.0,   1.0, 0.0,
                     pos.x, pos.y, 0.0, 1.0);
  // clang-format on

  return translation * transformation();
}

const Color& Entity::backgroundColor() const {
  return _backgroundColor;
}

void Entity::setBackgroundColor(const Color& backgroundColor) {
  _backgroundColor = backgroundColor;
  notifyInterfaceIfNecessary(BackgroundColor);
}

double Entity::opacity() const {
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

}  // namespace rl
