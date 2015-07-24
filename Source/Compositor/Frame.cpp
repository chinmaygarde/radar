// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "Compositor/Frame.h"

#include <cassert>

#define RL_OPENGL_ALLOWED 1
#include "Compositor/OpenGL.h"

using namespace rl;

Frame::Frame(Matrix projectionMatrix, std::shared_ptr<ProgramCatalog> catalog)
    : _projectionMatrix(projectionMatrix), _programCatalog(catalog) {
  assert(catalog != nullptr && "The program catalog must be valid");
}

void Frame::begin() {
  _programCatalog->startUsing();
  startNewFrame();
}

void Frame::end() {
  _programCatalog->stopUsing();
}

void Frame::startNewFrame() {
  glClearColor(0.0, 0.0, 0.0, 1.0);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
}
