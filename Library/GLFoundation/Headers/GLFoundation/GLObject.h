/*
 *  This source file is part of the Radar project.
 *  Licensed under the MIT License. See LICENSE file for details.
 */

#pragma once

#include <Core/Handle.h>
#include <GLFoundation/OpenGL.h>

namespace rl {
namespace gl {

struct GLFramebufferTraits {
  static GLuint NullHandle() { return GL_NONE; };
  static bool IsValid(GLuint handle) { return handle >= GL_NONE; }
  static bool CollectHandle(GLuint handle) {
    ::glDeleteFramebuffers(1, &handle);
    return true;
  }
};

struct GLRenderbufferTraits {
  static GLuint NullHandle() { return GL_NONE; };
  static bool IsValid(GLuint handle) { return handle >= GL_NONE; }
  static bool CollectHandle(GLuint handle) {
    ::glDeleteRenderbuffers(1, &handle);
    return true;
  }
};

struct GLTextureTraits {
  static GLuint NullHandle() { return GL_NONE; };
  static bool IsValid(GLuint handle) { return handle >= GL_NONE; }
  static bool CollectHandle(GLuint handle) {
    ::glDeleteTextures(1, &handle);
    return true;
  }
};

using GLFramebuffer = core::Handle<GLuint, GLFramebufferTraits>;
using GLRenderbuffer = core::Handle<GLuint, GLRenderbufferTraits>;
using GLTexture = core::Handle<GLuint, GLTextureTraits>;

}  // namespace gl
}  // namespace rl
