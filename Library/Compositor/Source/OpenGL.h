// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef RADARLOVE_COMPOSITOR_OPENGL_
#define RADARLOVE_COMPOSITOR_OPENGL_

#include <Core/Config.h>

#if RL_OS_MAC
#include <OpenGL/gl.h>
#elif RL_OS_LINUX || RL_OS_NACL || RL_OS_BSD
#include <GLES2/gl2.h>
#else
#error Unsupported OS
#endif

#include <Geometry/Geometry.h>

#include <stdio.h>
#include <string>
#include <sstream>

namespace rl {
namespace compositor {

void GLAssertError(const char* file, int line, const char* fmt...);
void GLDescribeFramebuffer(void);

struct GLCoord {
  GLfloat x;
  GLfloat y;
  GLfloat z;
};

struct GLMatrix {
  GLfloat m[16];
  // clang-format off
  GLMatrix(const geom::Matrix& o)
      : m{
            static_cast<GLfloat>(o.m[0]),  static_cast<GLfloat>(o.m[1]),
            static_cast<GLfloat>(o.m[2]),  static_cast<GLfloat>(o.m[3]),
            static_cast<GLfloat>(o.m[4]),  static_cast<GLfloat>(o.m[5]),
            static_cast<GLfloat>(o.m[6]),  static_cast<GLfloat>(o.m[7]),
            static_cast<GLfloat>(o.m[8]),  static_cast<GLfloat>(o.m[9]),
            static_cast<GLfloat>(o.m[10]), static_cast<GLfloat>(o.m[11]),
            static_cast<GLfloat>(o.m[12]), static_cast<GLfloat>(o.m[13]),
            static_cast<GLfloat>(o.m[14]), static_cast<GLfloat>(o.m[15]),
        } {};
  // clang-format on
};

}  // namespace compositor
}  // namespace rl

static_assert(sizeof(rl::compositor::GLMatrix) == 16 * sizeof(GLfloat),
              "GLMatrix must have the expected size");

#define RL_GLAssert(x, ...) \
  rl::compositor::GLAssertError(__FILE__, __LINE__, (x), ##__VA_ARGS__);

#endif  // RADARLOVE_COMPOSITOR_OPENGL_
