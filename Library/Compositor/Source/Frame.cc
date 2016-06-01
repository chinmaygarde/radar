// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <Core/Config.h>

#if !RL_OS_WINDOWS

#include <Core/Utilities.h>
#include <Compositor/Frame.h>
#include <Compositor/Primitive.h>

#include "OpenGL.h"
#include "BoxVertices.h"

namespace rl {
namespace compositor {

Frame::Frame(geom::Size size,
             std::shared_ptr<ProgramCatalog> catalog,
             CoordinatorStatistics& stats)
    : _size(size),
      _projectionMatrix(geom::Matrix::Orthographic(size)),
      _programCatalog(catalog),
      _stats(stats),
      _unitBoxVertices(
          core::make_unique<BoxVertices>(geom::Rect{0.0, 0.0, 1.0, 1.0})) {
  RL_ASSERT_MSG(catalog != nullptr, "The program catalog must be valid");
}

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

const geom::Matrix& Frame::projectionMatrix() const {
  return _projectionMatrix;
}

std::shared_ptr<ProgramCatalog> Frame::programCatalog() const {
  return _programCatalog;
}

CoordinatorStatistics& Frame::statistics() {
  return _stats;
}

BoxVertices& Frame::unitBoxVertices() {
  return *_unitBoxVertices;
}

Frame::~Frame() {}

}  // namespace compositor
}  // namespace rl

#endif  // !RL_OS_WINDOWS