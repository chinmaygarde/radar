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
  GLuint framebuffer = GL_NONE;
  glGenFramebuffers(1, &framebuffer);
  if (framebuffer == GL_NONE) {
    return false;
  }
  _framebuffer.reset(framebuffer);

  /*
   *  Generate the renderbuffer.
   */
  GLuint renderbuffer = GL_NONE;
  glGenRenderbuffers(1, &renderbuffer);
  if (renderbuffer == GL_NONE) {
    return false;
  }
  _renderbuffer.reset(renderbuffer);

  /*
   *  Setup the framebuffer.
   */
  glBindFramebuffer(GL_FRAMEBUFFER, _framebuffer);
  glBindRenderbuffer(GL_RENDERBUFFER, _renderbuffer);
  glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
                            GL_RENDERBUFFER, _renderbuffer);
  glRenderbufferStorage(GL_RENDERBUFFER, GL_RGBA4, size.width, size.height);

  RL_GLAssert("There must be no errors post framebuffer setup.");

  clearCurrent();

  return true;
}

bool GraphicsConnectionMac::isValid() const {
  return _context && _framebuffer != GL_NONE && _renderbuffer != GL_NONE;
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

  glBindFramebuffer(GL_FRAMEBUFFER, _framebuffer);
  return true;
}

}  // namespace testrunner
}  // namespace rl

#endif  // RL_OS_MAC
