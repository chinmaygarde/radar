// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef __RADARLOVE_COMPOSITOR_OPENGL__
#define __RADARLOVE_COMPOSITOR_OPENGL__

#if !RL_OPENGL_ALLOWED
#error Importing OpenGL Header. Are you sure your source file needs this invasive header?
#endif

#import <OpenGL/gl.h>

#include "Geometry/Matrix.h"

#include <cassert>
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
}

static_assert(sizeof(rl::GLMatrix) == 16 * sizeof(GLfloat),
              "GLMatrix must have the expected size");

#define RL_GLAssert(x, ...) \
  rl::GLAssertError(__FILE__, __LINE__, (x), ##__VA_ARGS__);

#endif /* defined(__RADARLOVE_COMPOSITOR_OPENGL__) */
