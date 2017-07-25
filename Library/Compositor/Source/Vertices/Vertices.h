/*
 *  This source file is part of the Radar project.
 *  Licensed under the MIT License. See LICENSE file for details.
 */

#pragma once

#include <Core/Macros.h>
#include <Core/ThreadGuard.h>
#include <GLFoundation/GLFoundation.h>

namespace rl {
namespace compositor {

class Vertices {
 public:
  enum class Type {
    Array,
    ElementArray,
  };

  Vertices(Type type);

  virtual ~Vertices();

  bool prepare();

  bool draw(size_t index) const;

 private:
  const Type _type;
  gl::GLResourceState _state;
  GLuint _vbo;
  GLuint _ibo;

  bool prepareBuffers();

  bool bindOrUnbind(bool bind) const;

  virtual bool doDraw(size_t index) const = 0;

  virtual bool uploadVertexData() = 0;

  RL_DISALLOW_COPY_AND_ASSIGN(Vertices);
};

}  // namespace compositor
}  // namespace rl
