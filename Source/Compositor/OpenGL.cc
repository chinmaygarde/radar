// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#define RL_OPENGL_ALLOWED 1
#include <Compositor/OpenGL.h>
#include <cassert>
#include <cstdarg>
#include <cstring>

namespace rl {

void GLAssertError(const char* file, int line, const char* fmt...) {
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
      message = "Unknown Error";
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

static const char* RL_GLDescribeFramebufferStatus(GLenum status) {
  switch (status) {
    case GL_FRAMEBUFFER_COMPLETE:
      return "GL_FRAMEBUFFER_COMPLETE";
    case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT:
      return "GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT";
#if GL_ES_VERSION_2_0
    case GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS:
      return "GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS";
#endif
    case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT:
      return "GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT";
    case GL_FRAMEBUFFER_UNSUPPORTED:
      return "GL_FRAMEBUFFER_UNSUPPORTED";
    case GL_INVALID_ENUM:
      return "GL_INVALID_ENUM";
  }

  return "Unknown FBO Status";
}

static const char* RL_GLAttachmentTypeString(GLint type) {
  switch (type) {
    case GL_RENDERBUFFER:
      return "GL_RENDERBUFFER";
    case GL_TEXTURE:
      return "GL_TEXTURE";
    case GL_NONE:
      return "GL_NONE";
  }

  return "Unknown Type";
}

static std::string RL_GLDescribeFramebufferAttachment(GLenum attachment) {
  GLint param = GL_NONE;
  glGetFramebufferAttachmentParameteriv(GL_FRAMEBUFFER, attachment,
                                        GL_FRAMEBUFFER_ATTACHMENT_OBJECT_TYPE,
                                        &param);

  if (param != GL_NONE) {
    const char* type = RL_GLAttachmentTypeString(param);

    param = GL_NONE;
    glGetFramebufferAttachmentParameteriv(GL_FRAMEBUFFER, attachment,
                                          GL_FRAMEBUFFER_ATTACHMENT_OBJECT_NAME,
                                          &param);

    std::stringstream stream;
    stream << type << "(" << param << ")";
    return stream.str();
  }

  return "No Attachment";
}

void GLDescribeFramebuffer(void) {
  if (glGetError() != GL_NO_ERROR) {
    printf("WARNING: There were already GL errors at this point...\n");
  }

  GLint framebuffer = GL_NONE;
  glGetIntegerv(GL_FRAMEBUFFER_BINDING, &framebuffer);

  char description[128] = {0};

  GLenum status = glCheckFramebufferStatus(framebuffer);

  snprintf(description, 128, "FBO %d%s %s", framebuffer,
           framebuffer == GL_NONE ? " (Default)" : "",
           RL_GLDescribeFramebufferStatus(status));

  printf("Describing %s\n", description);

  // Color Attachment
  printf("%s : Color Attachment : %s\n", description,
         RL_GLDescribeFramebufferAttachment(GL_COLOR_ATTACHMENT0).c_str());

  // Depth Attachment
  printf("%s : Depth Attachment : %s\n", description,
         RL_GLDescribeFramebufferAttachment(GL_DEPTH_ATTACHMENT).c_str());

  // Stencil Attachment
  printf("%s : Stencil Attachment : %s\n", description,
         RL_GLDescribeFramebufferAttachment(GL_STENCIL_ATTACHMENT).c_str());

  // Clear any GL errors that we introduced as part of logging
  glGetError();
}

}  // namespace rl
