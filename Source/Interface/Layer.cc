// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "Interface/Layer.h"
#include "Interface/Interface.h"

#include <cassert>

namespace rl {

#define MarkPatch(x) Interface::current().transaction().patch().mark(*this, (x))

/*
 *  TODO: There needs to be a better way to denote layer identity when
 *  communicating with the compositor. Maybe a good first step would be to make
 *  this interface local.
 */
static PatchChunk::Identifier CurrentPatchIdentifier = 0;

Layer::Layer()
    : _patchIdentifier(++CurrentPatchIdentifier),
      _bounds(RectZero),
      _position(PointZero),
      _anchorPoint(Point(0.5, 0.5)),
      _transformation(MatrixIdentity),
      _modelMatrix(MatrixIdentity),
      _modelMatrixDirty(true),
      _backgroundColor(ColorWhiteTransparent),
      _opacity(1.0),
      _sublayers(),
      _superlayer(nullptr) {
  MarkPatch(PatchChunk::Command::Created);
}

Layer::~Layer() {
  MarkPatch(PatchChunk::Command::Destroyed);
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
  _modelMatrixDirty = true;
  MarkPatch(PatchChunk::Command::Bounds);
}

const Point& Layer::position() const {
  return _position;
}

void Layer::setPosition(const Point& position) {
  if (_position == position) {
    return;
  }

  _position = position;
  _modelMatrixDirty = true;
  MarkPatch(PatchChunk::Command::Position);
}

const Point& Layer::anchorPoint() const {
  return _anchorPoint;
}

void Layer::setAnchorPoint(const Point& anchorPoint) {
  if (_anchorPoint == anchorPoint) {
    return;
  }

  _anchorPoint = anchorPoint;
  _modelMatrixDirty = true;
  MarkPatch(PatchChunk::Command::AnchorPoint);
}

const Matrix& Layer::transformation() const {
  return _transformation;
}

void Layer::setTransformation(const Matrix& transformation) {
  if (_transformation == transformation) {
    return;
  }

  _transformation = transformation;
  _modelMatrixDirty = true;
  MarkPatch(PatchChunk::Command::Matrix);
}

const Matrix& Layer::modelMatrix() {
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

const Color& Layer::backgroundColor() const {
  return _backgroundColor;
}

void Layer::setBackgroundColor(const Color& backgroundColor) {
  if (_backgroundColor == backgroundColor) {
    return;
  }

  _backgroundColor = backgroundColor;

  MarkPatch(PatchChunk::Command::Color);
}

double Layer::opacity() const {
  return _opacity;
}

void Layer::setOpacity(double opacity) {
  if (_opacity == opacity) {
    return;
  }

  MarkPatch(PatchChunk::Command::Opacity);
  _opacity = opacity;
}

PatchChunk::Identifier Layer::patchIdentifier() const {
  return _patchIdentifier;
}

}  // namespace rl
