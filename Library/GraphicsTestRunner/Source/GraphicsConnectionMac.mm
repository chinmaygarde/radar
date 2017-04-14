/*
 *  This source file is part of the Radar project.
 *  Licensed under the MIT License. See LICENSE file for details.
 */

#include <Core/Platform.h>
#include "GraphicsConnectionMac.h"

#if RL_OS_MAC

#import <AppKit/AppKit.h>

namespace rl {
namespace testrunner {

GraphicsConnectionMac::GraphicsConnectionMac(const geom::Size& size) {
  if (!setupContext()) {
    return;
  }

  if (!setupFramebuffer(size)) {
    return;
  }
}

GraphicsConnectionMac::~GraphicsConnectionMac() = default;

bool GraphicsConnectionMac::setupContext() {
  const NSOpenGLPixelFormatAttribute attributes[] = {
      NSOpenGLPFAColorSize, 32, 0,
  };

  core::ScopedNSObject<NSOpenGLPixelFormat> pixelFormat(
      [[NSOpenGLPixelFormat alloc] initWithAttributes:attributes]);

  if (!pixelFormat) {
    return false;
  }

  core::ScopedNSObject<NSOpenGLContext> context([[NSOpenGLContext alloc]
      initWithFormat:pixelFormat
        shareContext:nullptr]);

  if (!context) {
    return false;
  }

  _context = std::move(context);
  return true;
}

bool GraphicsConnectionMac::setupFramebuffer(const geom::Size& size) {
  if (!makeCurrent()) {
    return false;
  }

  /*
   *  Generate the framebuffer.
   */
  {
    GLuint framebuffer = GL_NONE;
    glGenFramebuffers(1, &framebuffer);
    if (framebuffer == GL_NONE) {
      return false;
    }
    _framebuffer.reset(framebuffer);
  }

  /*
   *  Generate the texture.
   */
  {
    GLuint texture = GL_NONE;
    glGenTextures(1, &texture);
    if (texture == GL_NONE) {
      return false;
    }
    _texture.reset(texture);
  }

  /*
   *  Configure the texture.
   */
  glBindTexture(GL_TEXTURE_2D, _texture);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, size.width, size.height, 0, GL_RGBA,
               GL_UNSIGNED_BYTE, nullptr);

  /*
   *  Set the texture as the color attachment of the framebuffer.
   */
  glBindFramebuffer(GL_FRAMEBUFFER, _framebuffer);
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D,
                         _texture, 0);

  RL_GLAssert("There must be no errors post framebuffer setup.");

  clearCurrent();

  return true;
}

bool GraphicsConnectionMac::isValid() const {
  return _context && _framebuffer != GL_NONE && _texture != GL_NONE;
}

bool GraphicsConnectionMac::makeCurrent() {
  if (!_context) {
    return false;
  }

  [_context makeCurrentContext];
  return true;
}

bool GraphicsConnectionMac::clearCurrent() {
  if ([NSOpenGLContext currentContext] != _context) {
    return false;
  }

  [NSOpenGLContext clearCurrentContext];
  return true;
}

bool GraphicsConnectionMac::activate() {
  if (!isValid()) {
    return false;
  }

  if (!makeCurrent()) {
    return false;
  }

  glBindFramebuffer(GL_FRAMEBUFFER, _framebuffer);
  return true;
}

bool GraphicsConnectionMac::deactivate() {
  if (!isValid()) {
    return false;
  }

  if (!clearCurrent()) {
    return false;
  }

  glBindFramebuffer(GL_FRAMEBUFFER, GL_NONE);
  return true;
}

}  // namespace testrunner
}  // namespace rl

#endif  // RL_OS_MAC
