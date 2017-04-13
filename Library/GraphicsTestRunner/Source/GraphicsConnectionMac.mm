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

GraphicsConnectionMac::GraphicsConnectionMac() {
  const NSOpenGLPixelFormatAttribute attributes[] = {
      NSOpenGLPFAColorSize, 32, 0,
  };

  core::ScopedNSObject<NSOpenGLPixelFormat> pixelFormat(
      [[NSOpenGLPixelFormat alloc] initWithAttributes:attributes]);

  if (!pixelFormat) {
    return;
  }

  core::ScopedNSObject<NSOpenGLContext> context([[NSOpenGLContext alloc]
      initWithFormat:pixelFormat
        shareContext:nullptr]);

  if (!context) {
    return;
  }

  _context = std::move(context);
}

GraphicsConnectionMac::~GraphicsConnectionMac() {}

bool GraphicsConnectionMac::isValid() const {
  return _context;
}

bool GraphicsConnectionMac::activate() {
  if (!_context) {
    return false;
  }

  [_context makeCurrentContext];
  return true;
}

bool GraphicsConnectionMac::deactivate() {
  if ([NSOpenGLContext currentContext] != _context) {
    return false;
  }

  [NSOpenGLContext clearCurrentContext];
  return true;
}

}  // namespace testrunner
}  // namespace rl

#endif  // RL_OS_MAC
