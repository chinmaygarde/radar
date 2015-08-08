// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <Compositor/PresentationLayer.h>

namespace rl {

PresentationLayer::PresentationLayer()
    : Layer(),
      _presentationSublayers(),
      _backgroundPrimitive(),
      _foregroundPrimitive() {
}

void PresentationLayer::drawInFrame(Frame& frame) {
  frame.pushOpacity(opacity());

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

  for (const auto& layer : _presentationSublayers) {
    layer->drawInFrame(frame);
  }

  frame.popViewMatrix();

  frame.popOpacity();
}

}  // namespace rl
