// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef __RADARLOVE_COMPOSITOR_OPENGL__
#define __RADARLOVE_COMPOSITOR_OPENGL__

#if !RL_OPENGL_ALLOWED
#error Importing OpenGL Header. Are you sure your source file needs this invasive header?
#endif

#import <OpenGL/gl.h>

#include <cassert>
#include <stdio.h>
#include <string>
#include <sstream>

namespace rl {
void GLAssertError(const char* file, int line, const char* fmt...);
void GLDescribeFramebuffer(void);
}

#define RL_GLAssert(x, ...) \
  rl::GLAssertError(__FILE__, __LINE__, (x), ##__VA_ARGS__);

#endif /* defined(__RADARLOVE_COMPOSITOR_OPENGL__) */
