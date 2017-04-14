/*
 *  This source file is part of the Radar project.
 *  Licensed under the MIT License. See LICENSE file for details.
 */

#pragma once

#include <Core/Macros.h>
#include <GLFoundation/OpenGL.h>

namespace rl {
namespace gl {

template <class Traits>
class GLObject {
 public:
  GLObject() : _handle(GL_NONE) {}

  GLObject(GLuint handle) : _handle(handle) {}

  ~GLObject() { reset(); }

  void reset(GLuint other = GL_NONE) {
    if (_handle != GL_NONE) {
      Traits::Destroy(_handle);
    }
    _handle = other;
  }

  operator GLuint() const { return _handle; }

 private:
  GLuint _handle;

  RL_DISALLOW_COPY_AND_ASSIGN(GLObject);
};

struct GLFramebufferTraits {
  static void Destroy(GLuint handle) { ::glDeleteFramebuffers(1, &handle); }
};

struct GLRenderbufferTraits {
  static void Destroy(GLuint handle) { ::glDeleteRenderbuffers(1, &handle); }
};

struct GLTextureTraits {
  static void Destroy(GLuint handle) { ::glDeleteTextures(1, &handle); }
};

using GLFramebuffer = GLObject<GLFramebufferTraits>;
using GLRenderbuffer = GLObject<GLRenderbufferTraits>;
using GLTexture = GLObject<GLTextureTraits>;

}  // namespace gl
}  // namespace rl
