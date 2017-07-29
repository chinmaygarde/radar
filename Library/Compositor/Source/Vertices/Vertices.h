/*
 *  This source file is part of the Radar project.
 *  Licensed under the MIT License. See LICENSE file for details.
 */

#pragma once

#include <Core/Macros.h>
#include <Core/ThreadGuard.h>
#include <GLFoundation/GLFoundation.h>
#include <limits>

namespace rl {
namespace compositor {

class Vertices;

class Vertices {
 public:
  enum class Type {
    Array,
    ElementArray,
  };

  Vertices(Type type);

  virtual ~Vertices();

  bool prepare();

 protected:
  class AutoVertexAttribDisable {
   public:
    AutoVertexAttribDisable(size_t index) : _index(index) {
      if (index != kInvalidIndex) {
        glEnableVertexAttribArray(index);
      }
    }

    AutoVertexAttribDisable(AutoVertexAttribDisable&& o) : _index(o._index) {
      o._index = kInvalidIndex;
    }

    ~AutoVertexAttribDisable() {
      if (_index != kInvalidIndex) {
        glDisableVertexAttribArray(_index);
      }
    }

   private:
    static const size_t kInvalidIndex = std::numeric_limits<size_t>::max();
    size_t _index;

    RL_DISALLOW_COPY_AND_ASSIGN(AutoVertexAttribDisable);
  };

  class AutoVertexBufferUnbind {
   public:
    AutoVertexBufferUnbind(const Vertices* vertices) : _vertices(nullptr) {
      if (vertices == nullptr) {
        return;
      }

      if (vertices->bindOrUnbind(true)) {
        _vertices = vertices;
      }
    }

    AutoVertexBufferUnbind(AutoVertexBufferUnbind&& o)
        : _vertices(o._vertices) {
      o._vertices = nullptr;
    }

    ~AutoVertexBufferUnbind() {
      if (_vertices != nullptr) {
        _vertices->bindOrUnbind(false);
      }
    }

    operator bool() const { return _vertices != nullptr; }

   private:
    const Vertices* _vertices;

    RL_DISALLOW_COPY_AND_ASSIGN(AutoVertexBufferUnbind);
  };

  RL_WARN_UNUSED_RESULT
  AutoVertexBufferUnbind bindBuffer() const;

  RL_WARN_UNUSED_RESULT
  AutoVertexAttribDisable enableAttribute(size_t index,
                                          GLint componentsPerVertex,
                                          GLenum typeOfComponent,
                                          GLsizei stride,
                                          GLsizei offset) const;

 private:
  const Type _type;
  gl::GLResourceState _state;
  GLuint _vbo;
  GLuint _ibo;

  bool prepareBuffers();

  bool bindOrUnbind(bool bind) const;

  virtual bool uploadVertexData() = 0;

  RL_DISALLOW_COPY_AND_ASSIGN(Vertices);
};

}  // namespace compositor
}  // namespace rl
