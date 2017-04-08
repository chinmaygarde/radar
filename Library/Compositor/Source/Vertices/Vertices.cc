/*
 *  This source file is part of the Radar project.
 *  Licensed under the MIT License. See LICENSE file for details.
 */

#include "Vertices/Vertices.h"

namespace rl {
namespace compositor {

Vertices::Vertices(Type type)
    : _type(type),
      _state(ResourceState::NotReady),
      _vbo(GL_NONE),
      _ibo(GL_NONE) {}

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
  if (!bindOrUnbind(true)) {
    return false;
  }

  /*
   *  Ask the implementation to upload vertex data.
   */
  return uploadVertexData();
}

bool Vertices::bindOrUnbind(bool bind) {
  auto vbo = _vbo;
  auto ibo = _ibo;

  if (!bind) {
    vbo = GL_NONE;
    ibo = GL_NONE;
  }

  switch (_type) {
    case Type::Array:
      glBindBuffer(GL_ARRAY_BUFFER, vbo);
      return true;
    case Type::ElementArray:
      glBindBuffer(GL_ARRAY_BUFFER, vbo);
      glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
      return true;
  }

  return false;
}

bool Vertices::draw(size_t index) {
  if (!prepare()) {
    return false;
  }

  if (!bindOrUnbind(true)) {
    return false;
  }

  bool drawn = doDraw(index);

  bindOrUnbind(false);

  return drawn;
}

}  // namespace compositor
}  // namespace rl
