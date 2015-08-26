// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <Compositor/Frame.h>
#include <Compositor/Primitive.h>

#define RL_OPENGL_ALLOWED 1
#include <Compositor/OpenGL.h>

namespace rl {

Frame::Frame(Size size,
             std::shared_ptr<ProgramCatalog> catalog,
             CompositorStatistics& stats)
    : _size(size),
      _projectionMatrix(Matrix::Orthographic(size)),
      _programCatalog(catalog),
      _stats(stats) {
  RL_ASSERT(catalog != nullptr && "The program catalog must be valid");
}

bool Frame::isReady() const {
  return glGetError() == GL_NO_ERROR && _size.width * _size.height > 0.0;
}

const Size& Frame::size() const {
  return _size;
}

void Frame::begin() {
  setupFreshFrame();

  _programCatalog->startUsing();
}

void Frame::end() {
  _programCatalog->stopUsing();
}

void Frame::setupFreshFrame() {
  glViewport(0, 0, _size.width, _size.height);
  glScissor(0, 0, _size.width, _size.height);

  glDisable(GL_CULL_FACE);
  glDisable(GL_DEPTH_TEST);
  /* glDisable(GL_BLEND); See TODO */
  glEnable(GL_SCISSOR_TEST); /* only limited to the viewport */

  glClearColor(1.0, 1.0, 1.0, 1.0);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

  /*
   *  TODO: The scope of this call is too wide
   */
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

const Matrix& Frame::projectionMatrix() const {
  return _projectionMatrix;
}

std::shared_ptr<ProgramCatalog> Frame::programCatalog() const {
  return _programCatalog;
}

CompositorStatistics& Frame::statistics() {
  return _stats;
}

Frame::~Frame() {
}

ScopedFrame::~ScopedFrame() {
  end();
}

}  // namespace rl
