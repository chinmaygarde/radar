// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "Compositor/Entity.h"

using namespace rl;

Entity::Entity()
    : _bounds(RectZero),
      _position(PointZero),
      _anchorPoint(Point(0.5, 0.5)),
      _transformation(MatrixIdentity),
      _modelMatrix(MatrixIdentity),
      _modelMatrixDirty(true){};

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
  if (_bounds == bounds) {
    return;
  }

  _bounds = bounds;
  _modelMatrixDirty = true;
}

const Point& Entity::position() const {
  return _position;
}

void Entity::setPosition(const Point& position) {
  if (_position == position) {
    return;
  }

  _position = position;
  _modelMatrixDirty = true;
}

const Point& Entity::anchorPoint() const {
  return _anchorPoint;
}

void Entity::setAnchorPoint(const Point& anchorPoint) {
  if (_anchorPoint == anchorPoint) {
    return;
  }

  _anchorPoint = anchorPoint;
  _modelMatrixDirty = true;
}

const Matrix& Entity::transformation() const {
  return _transformation;
}

void Entity::setTransformation(const Matrix& transformation) {
  if (_transformation == transformation) {
    return;
  }

  _transformation = transformation;
  _modelMatrixDirty = true;
}

const Matrix& Entity::modelMatrix() {
  if (!_modelMatrixDirty) {
    return _modelMatrix;
  }

  _modelMatrixDirty = false;

  const Point pos(_position.x - _anchorPoint.x * _bounds.size.width,
                  _position.y - _anchorPoint.y * _bounds.size.height);

  // clang-format off
  Matrix translation(1.0,   0.0,   0.0, 0.0,
                     0.0,   1.0,   0.0, 0.0,
                     0.0,   0.0,   1.0, 0.0,
                     pos.x, pos.y, 0.0, 1.0);
  // clang-format on

  _modelMatrix = translation * transformation();

  return _modelMatrix;
}
