// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <Interface/Entity.h>

namespace rl {

Entity::Entity()
    : _bounds(RectZero),
      _position(PointZero),
      _anchorPoint(Point(0.5, 0.5)),
      _transformation(MatrixIdentity),
      _backgroundColor(ColorWhiteTransparent),
      _opacity(1.0) {
}

Entity::~Entity() {
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
}

const Point& Entity::position() const {
  return _position;
}

void Entity::setPosition(const Point& position) {
  _position = position;
}

const Point& Entity::anchorPoint() const {
  return _anchorPoint;
}

void Entity::setAnchorPoint(const Point& anchorPoint) {
  _anchorPoint = anchorPoint;
}

const Matrix& Entity::transformation() const {
  return _transformation;
}

void Entity::setTransformation(const Matrix& transformation) {
  _transformation = transformation;
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
}

double Entity::opacity() const {
  return _opacity;
}

void Entity::setOpacity(double opacity) {
  _opacity = opacity;
}

}  // namespace rl
