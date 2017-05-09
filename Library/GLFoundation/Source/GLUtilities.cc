/*
 *  This source file is part of the Radar project.
 *  Licensed under the MIT License. See LICENSE file for details.
 */

#include <Core/Macros.h>
#include <GLFoundation/GLUtilities.h>
#include <GLFoundation/OpenGL.h>
#include <sstream>
#include <string>

namespace rl {
namespace gl {

void AssertError(const char* file, int line, const char* fmt...) {
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
  vsnprintf(userMessage, sizeof(userMessage), fmt, args);
  va_end(args);

  const char* basename = (strrchr(file, '/') ? strrchr(file, '/') + 1 : file);

  RL_LOG("OpenGL Error (%s:%d): %s (%x) '%s'", basename, line, message.c_str(),
         res, userMessage);
  RL_ASSERT(false);
}

static const char* DescribeFramebufferStatus(GLenum status) {
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

  return "Unknown FBO Error Status";
}

static const char* AttachmentTypeString(GLint type) {
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

static std::string DescribeFramebufferAttachment(GLenum attachment) {
  GLint param = GL_NONE;
  glGetFramebufferAttachmentParameteriv(GL_FRAMEBUFFER, attachment,
                                        GL_FRAMEBUFFER_ATTACHMENT_OBJECT_TYPE,
                                        &param);

  if (param != GL_NONE) {
    const char* type = AttachmentTypeString(param);

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

bool IsFramebufferComplete(void) {
  GLint framebuffer = GL_NONE;
  glGetIntegerv(GL_FRAMEBUFFER_BINDING, &framebuffer);
  GLenum status = glCheckFramebufferStatus(framebuffer);
  return status == GL_FRAMEBUFFER_COMPLETE;
}

void DescribeFramebuffer(void) {
  if (glGetError() != GL_NO_ERROR) {
    RL_LOG("WARNING: There were already GL errors at this point...");
  }

  GLint framebuffer = GL_NONE;
  glGetIntegerv(GL_FRAMEBUFFER_BINDING, &framebuffer);

  char description[128] = {0};

  GLenum status = glCheckFramebufferStatus(framebuffer);

  snprintf(description, sizeof(description), "FBO %d%s %s", framebuffer,
           framebuffer == GL_NONE ? " (Default)" : "",
           DescribeFramebufferStatus(status));

  RL_LOG("Describing %s", description);

  // Color Attachment
  RL_LOG("%s : Color Attachment : %s", description,
         DescribeFramebufferAttachment(GL_COLOR_ATTACHMENT0).c_str());

  // Depth Attachment
  RL_LOG("%s : Depth Attachment : %s", description,
         DescribeFramebufferAttachment(GL_DEPTH_ATTACHMENT).c_str());

  // Stencil Attachment
  RL_LOG("%s : Stencil Attachment : %s", description,
         DescribeFramebufferAttachment(GL_STENCIL_ATTACHMENT).c_str());

  // Clear any GL errors that we introduced as part of logging
  glGetError();
}

}  // namespace gl
}  // namespace rl
