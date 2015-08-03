// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#import "Compositor/Layer.h"

#include <cassert>

namespace rl {

Layer::Layer()
    : Entity(),
      _backgroundColor(ColorWhiteTransparent),
      _opacity(1.0),
      _sublayers(),
      _superlayer(nullptr),
      _backgroundPrimitive(),
      _foregroundPrimitive() {
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

void Layer::drawInFrame(Frame& frame) {
  frame.pushOpacity(_opacity);

  /*
   *  Step 1:
   *  Render all primitives that are a part of this layer
   */
  if (_backgroundPrimitive) {
    _backgroundPrimitive->setModelMatrixAndSize(modelMatrix(), bounds().size);
    _backgroundPrimitive->render(frame);
  }

  if (_foregroundPrimitive) {
    _foregroundPrimitive->setModelMatrixAndSize(modelMatrix(), bounds().size);
    _foregroundPrimitive->render(frame);
  }

  /*
   *  Step 2:
   *  Recursively visit sublayers and render as needed
   */

  frame.pushViewMatrix(frame.viewMatrix() * modelMatrix());

  for (const auto& layer : _sublayers) {
    layer->drawInFrame(frame);
  }

  frame.popViewMatrix();

  frame.popOpacity();
}

}  // namespace rl
