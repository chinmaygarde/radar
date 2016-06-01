// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "Vertices.h"

namespace rl {
namespace compositor {

Vertices::Vertices() : _prepared(false), _disposed(false), _vbo(0) {}

Vertices::~Vertices() {
  RL_ASSERT_MSG(_disposed,
                "Vertices cannot be collected before being disposed");
}

GLuint Vertices::vbo() const {
  return _vbo;
}

bool Vertices::prepare() {
  if (_prepared) {
    return true;
  }

  if (_disposed) {
    return false;
  }

  if (_vbo == 0) {
    glGenBuffers(1, &_vbo);
    RL_ASSERT(_vbo != 0);
  }

  _prepared = uploadVertexData();
  return _prepared;
}

bool Vertices::dispose() {
  if (_disposed) {
    RL_ASSERT(_vbo == 0);
    return true;
  }

  if (!_prepared) {
    RL_ASSERT(_vbo == 0);

    _disposed = true;
    return true;
  }

  glDeleteBuffers(1, &_vbo);
  _vbo = 0;

  _disposed = true;
  return true;
}

}  // namespace compositor
}  // namespace rl
