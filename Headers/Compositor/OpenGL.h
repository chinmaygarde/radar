// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef RADARLOVE_COMPOSITOR_OPENGL_
#define RADARLOVE_COMPOSITOR_OPENGL_

#if !RL_OPENGL_ALLOWED
/*
 *  An OpenGL backend to the compositor is merely a current implementation
 *  OpenGL headers and types have the alarming tendency to get included in
 *  places they really shouldn't be. If you hit this error, think about if that
 *  file really needs this. Knowing that the file would need
 *  rewrites/modifications for other backend.
 */
#error Importing OpenGL Header. This is an invasive header
#endif

#include <Core/Config.h>

#if RL_OS_MAC
#include <OpenGL/gl.h>
#elif RL_OS_LINUX || RL_OS_NACL
#include <GLES2/gl2.h>
#else
#error Unsupported OS
#endif

#include <Geometry/Geometry.h>

#include <stdio.h>
#include <string>
#include <sstream>

namespace rl {
void GLAssertError(const char* file, int line, const char* fmt...);
void GLDescribeFramebuffer(void);

struct GLCoord {
  GLfloat x;
  GLfloat y;
  GLfloat z;
};

struct GLMatrix {
  GLfloat m[16];
  GLMatrix(const Matrix& o)
      : m{
            static_cast<GLfloat>(o.m[0]),
            static_cast<GLfloat>(o.m[1]),
            static_cast<GLfloat>(o.m[2]),
            static_cast<GLfloat>(o.m[3]),
            static_cast<GLfloat>(o.m[4]),
            static_cast<GLfloat>(o.m[5]),
            static_cast<GLfloat>(o.m[6]),
            static_cast<GLfloat>(o.m[7]),
            static_cast<GLfloat>(o.m[8]),
            static_cast<GLfloat>(o.m[9]),
            static_cast<GLfloat>(o.m[10]),
            static_cast<GLfloat>(o.m[11]),
            static_cast<GLfloat>(o.m[12]),
            static_cast<GLfloat>(o.m[13]),
            static_cast<GLfloat>(o.m[14]),
            static_cast<GLfloat>(o.m[15]),
        } {};
};
}  // namespace rl

static_assert(sizeof(rl::GLMatrix) == 16 * sizeof(GLfloat),
              "GLMatrix must have the expected size");

#define RL_GLAssert(x, ...) \
  rl::GLAssertError(__FILE__, __LINE__, (x), ##__VA_ARGS__);

#endif  // RADARLOVE_COMPOSITOR_OPENGL_
