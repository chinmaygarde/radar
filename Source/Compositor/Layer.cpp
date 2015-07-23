// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#import "Compositor/Layer.h"

#include <cassert>

using namespace rl;

Layer::Layer()
    : _bounds(RectZero),
      _position(PointZero),
      _anchorPoint(Point(0.5, 0.5)),
      _transformation(MatrixIdentity),
      _backgroundColor(ColorWhiteTransparent),
      _opacity(1.0),
      _sublayers(),
      _superlayer(nullptr),
      _backgroundPrimitive(),
      _foregroundPrimitive() {
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
  if (_bounds == bounds) {
    return;
  }

  _bounds = bounds;
}

const Point& Layer::position() const {
  return _position;
}

void Layer::setPosition(const Point& position) {
  if (_position == position) {
    return;
  }

  _position = position;
}

const Point& Layer::anchorPoint() const {
  return _anchorPoint;
}

void Layer::setAnchorPoint(const Point& anchorPoint) {
  if (_anchorPoint == anchorPoint) {
    return;
  }

  _anchorPoint = anchorPoint;
}

const Matrix& Layer::transformation() const {
  return _transformation;
}

void Layer::setTransformation(const Matrix& transformation) {
  if (_transformation == transformation) {
    return;
  }

  _transformation = transformation;
}

void Layer::addSublayer(Layer::LayerRef layer) {
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

void Layer::removeSublayer(Layer::LayerRef layer) {
  if (layer == nullptr) {
    return;
  }

  auto found = std::find(_sublayers.begin(), _sublayers.end(), layer);

  if (found != _sublayers.end()) {
    _sublayers.erase(found);
    layer->_superlayer = nullptr;
  }
}

const Color& Layer::backgroundColor() const {
  return _backgroundColor;
}

void Layer::setBackgroundColor(const Color& backgroundColor) {
  if (_backgroundColor == backgroundColor) {
    return;
  }

  _backgroundColor = backgroundColor;

  if (backgroundColor.a < TransparencyAlphaThreshold) {
    _backgroundPrimitive = nullptr;
  } else {
    if (!_backgroundPrimitive) {
      _backgroundPrimitive = Utils::make_unique<Primitive>();
    }
    _backgroundPrimitive->setContentColor(_backgroundColor);
  }
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

void Layer::drawInFrame(const Frame& frame) {
  if (_backgroundPrimitive) {
    _backgroundPrimitive->setSizeAndModelViewMatrix(_bounds.size,
                                                    MatrixIdentity);
    _backgroundPrimitive->render(frame);
  }

  if (_foregroundPrimitive) {
    _backgroundPrimitive->setSizeAndModelViewMatrix(_bounds.size,
                                                    MatrixIdentity);
    _foregroundPrimitive->render(frame);
  }

  for (const auto& layer : _sublayers) {
    layer->drawInFrame(frame);
  }
}

Matrix Layer::modelViewMatrix(const Matrix& viewMatrix) const {
  /*
   *  Apply transformation about the anchor point
   */
  const Matrix transformed = _transformation - (_anchorPoint * _bounds.size);

  /*
   *  Apply the offset supplied in the bounds
   */
  const Matrix positioned =
      Matrix::Translation(_position - _bounds.origin) * transformed;

  /*
   *  Apply the view matrix to the transformed and positioned matrix
   */
  const Matrix resultant = viewMatrix * positioned;

  /*
   *  Generate the model view matrix
   */
  return resultant + _bounds.origin;
}
