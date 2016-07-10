// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "Vertices.h"

namespace rl {
namespace compositor {

Vertices::Vertices(Type type) : _type(type), _vbo(GL_NONE) {}

Vertices::~Vertices() {
  if (_vbo != GL_NONE) {
    glDeleteBuffers(1, &_vbo);
    _vbo = GL_NONE;
  }
}

static GLuint CreateVBO() {
  GLuint buffer = GL_NONE;
  glGenBuffers(1, &buffer);
  return buffer;
}

bool Vertices::prepare() {
  switch (_state) {
    case ResourceState::Ready:
      return true;
    case ResourceState::Failed:
      return false;
    case ResourceState::NotReady:
      _vbo = CreateVBO();

      if (_vbo == GL_NONE || !use()) {
        _state = ResourceState::Failed;
        return false;
      }

      bool uploaded = uploadVertexData();
      _state = uploaded ? ResourceState::Ready : ResourceState::Failed;
      return uploaded;
  }

  return false;
}

bool Vertices::use() {
  switch (_type) {
    case Type::Array:
      glBindBuffer(GL_ARRAY_BUFFER, _vbo);
      return true;
    case Type::ElementArray:
      glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _vbo);
      return true;
  }
  return false;
}

bool Vertices::draw(size_t index) {
  if (!prepare()) {
    return false;
  }

  if (!use()) {
    return false;
  }

  return doDraw(index);
}

}  // namespace compositor
}  // namespace rl
