// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <Core/Utilities.h>
#include <Compositor/Frame.h>
#include <Compositor/Primitive/Primitive.h>
#include <Compositor/Context.h>

#include "OpenGL.h"

namespace rl {
namespace compositor {

Frame::Frame(geom::Size size, Context& context)
    : _size(size),
      _projectionMatrix(geom::Matrix::Orthographic(size)),
      _context(context) {}

Frame::~Frame() = default;

bool Frame::isReady() const {
  if (_size.width <= 0.0 && _size.height <= 0.0) {
    return false;
  }

  if (glGetError() != GL_NO_ERROR) {
    return false;
  }

  return true;
}

const geom::Size& Frame::size() const {
  return _size;
}

bool Frame::begin() {
  if (!_context.beginUsing()) {
    return false;
  }

  prepareFrame();

  return true;
}

bool Frame::end() {
  renderStatistics();

  return _context.endUsing();
}

void Frame::prepareFrame() {
  glViewport(0, 0, _size.width, _size.height);

  glScissor(0, 0, _size.width, _size.height);
  glEnable(GL_SCISSOR_TEST);

  glDisable(GL_CULL_FACE);

  glDisable(GL_DEPTH_TEST);

  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  glClearColor(1.0, 1.0, 1.0, 1.0);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
}

void Frame::renderStatistics() {
  _context.renderStatistics(*this);
}

Context& Frame::context() {
  return _context;
}

const geom::Matrix& Frame::projectionMatrix() const {
  return _projectionMatrix;
}

}  // namespace compositor
}  // namespace rl
