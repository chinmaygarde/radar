// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <Interface/Layer.h>
#include <Interface/Interface.h>

#include <cassert>

namespace rl {

Layer::Layer()
    : _bounds(RectZero),
      _position(PointZero),
      _anchorPoint(Point(0.5, 0.5)),
      _transformation(MatrixIdentity),
      _modelMatrix(MatrixIdentity),
      _backgroundColor(ColorWhiteTransparent),
      _opacity(1.0),
      _sublayers(),
      _superlayer(nullptr) {
}

Layer::~Layer() {
}

Rect Layer::frame() const {
  Point origin(_position.x - (_bounds.size.width * _anchorPoint.x),
               _position.y - (_bounds.size.height * _anchorPoint.y));

  return Rect(origin, _bounds.size);
}

void Layer::setFrame(const Rect& frame) {
  setBounds(Rect(_bounds.origin, frame.size));
  setPosition(Point(frame.origin.x + (_anchorPoint.x * frame.size.width),
                    frame.origin.y + (_anchorPoint.y * frame.size.height)));
}

const Rect& Layer::bounds() const {
  return _bounds;
}

void Layer::setBounds(const Rect& bounds) {
  _bounds = bounds;
}

const Point& Layer::position() const {
  return _position;
}

void Layer::setPosition(const Point& position) {
  _position = position;
}

const Point& Layer::anchorPoint() const {
  return _anchorPoint;
}

void Layer::setAnchorPoint(const Point& anchorPoint) {
  _anchorPoint = anchorPoint;
}

const Matrix& Layer::transformation() const {
  return _transformation;
}

void Layer::setTransformation(const Matrix& transformation) {
  _transformation = transformation;
}

Matrix Layer::modelMatrix() const {
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

void Layer::addSublayer(Layer::Ref layer) {
  if (layer == nullptr) {
    return;
  }

  /**
   *  Make sure this layer has no parent to begin with
   */
  if (layer->_superlayer != nullptr) {
    layer->_superlayer->removeSublayer(layer);
  }
  assert(layer->_superlayer == nullptr);

  /*
   *  Assign the parent and add the same to the sublayers collections
   */
  layer->_superlayer = this;
  _sublayers.push_back(layer);
}

void Layer::removeSublayer(Layer::Ref layer) {
  if (layer == nullptr) {
    return;
  }

  auto found = std::find(_sublayers.begin(), _sublayers.end(), layer);

  if (found != _sublayers.end()) {
    _sublayers.erase(found);
    layer->_superlayer = nullptr;
  }
}

std::list<Layer::Ref> Layer::sublayers() const {
  return _sublayers;
}

const Layer* Layer::superlayer() const {
  return _superlayer;
}

const Color& Layer::backgroundColor() const {
  return _backgroundColor;
}

void Layer::setBackgroundColor(const Color& backgroundColor) {
  if (_backgroundColor == backgroundColor) {
    return;
  }

  _backgroundColor = backgroundColor;
}

double Layer::opacity() const {
  return _opacity;
}

void Layer::setOpacity(double opacity) {
  if (_opacity == opacity) {
    return;
  }

  _opacity = opacity;
}

}  // namespace rl
