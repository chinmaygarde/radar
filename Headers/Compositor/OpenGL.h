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

inline void RLAssertGLError(const char* file, int line, const char* fmt...) {
  GLenum res = glGetError();
  if (res == GL_NO_ERROR) {
    return;
  }

  std::string message = "";
  switch (res) {
    case GL_INVALID_ENUM:
      message = "GL_INVALID_ENUM";
      break;
    case GL_INVALID_VALUE:
      message = "GL_INVALID_VALUE";
      break;
    case GL_INVALID_OPERATION:
      message = "GL_INVALID_OPERATION";
      break;
    case GL_OUT_OF_MEMORY:
      message = "GL_OUT_OF_MEMORY";
      break;
    case GL_INVALID_FRAMEBUFFER_OPERATION:
      message = "GL_INVALID_FRAMEBUFFER_OPERATION";
      break;
    default:
      message = "Unknown";
      break;
  }

  char userMessage[128] = {0};

  va_list args;
  va_start(args, fmt);
  vsnprintf(userMessage, 128, fmt, args);
  va_end(args);

  const char* basename = (strrchr(file, '/') ? strrchr(file, '/') + 1 : file);

  printf("OpenGL Error (%s:%d): %s (%x) '%s'\n", basename, line,
         message.c_str(), res, userMessage);
  assert(false);
}

#define RL_GLAssert(x, ...) \
  RLAssertGLError(__FILE__, __LINE__, (x), ##__VA_ARGS__);

#endif /* defined(__RADARLOVE_COMPOSITOR_OPENGL__) */
