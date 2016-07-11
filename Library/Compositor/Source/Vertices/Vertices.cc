// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "Vertices/Vertices.h"

namespace rl {
namespace compositor {

Vertices::Vertices(Type type) : _type(type), _vbo(GL_NONE), _ibo(GL_NONE) {}

Vertices::~Vertices() {
  if (_vbo != GL_NONE) {
    glDeleteBuffers(1, &_vbo);
    _vbo = GL_NONE;
  }

  if (_ibo != GL_NONE) {
    glDeleteBuffers(1, &_ibo);
    _ibo = GL_NONE;
  }
}

static GLuint CreateBuffer() {
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
      if (prepareBuffers()) {
        _state = ResourceState::Ready;
        return true;
      } else {
        _state = ResourceState::Failed;
        return false;
      }
  }

  RL_ASSERT(false);
  return false;
}

bool Vertices::prepareBuffers() {
  RL_ASSERT(_vbo == GL_NONE);
  RL_ASSERT(_ibo == GL_NONE);
  /*
   *  Always try to create a vertex buffer.
   */
  _vbo = CreateBuffer();
  if (_vbo == GL_NONE) {
    return false;
  }

  /*
   *  Create an index buffer if necessary.
   */
  if (_type == Type::ElementArray) {
    _ibo = CreateBuffer();
    if (_ibo == GL_NONE) {
      return false;
    }
  }

  /*
   *  Prepare buffer bindinds before attempting to upload vertices.
   */
  if (!use()) {
    return false;
  }

  /*
   *  Ask the implementation to upload vertex data.
   */
  return uploadVertexData();
}

bool Vertices::use() {
  switch (_type) {
    case Type::Array:
      glBindBuffer(GL_ARRAY_BUFFER, _vbo);
      return true;
    case Type::ElementArray:
      glBindBuffer(GL_ARRAY_BUFFER, _vbo);
      glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ibo);
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
