/*
 *  This source file is part of the Radar project.
 *  Licensed under the MIT License. See LICENSE file for details.
 */

#include "EGLContext.h"
#include "EGLUtils.h"

namespace rl {
namespace testrunner {

EGLContext::EGLContext()
    : _isValid(false), _display(nullptr), _context(nullptr) {}

EGLContext::EGLContext(EGLContext&& o)
    : _isValid(o._isValid), _display(o._display), _context(o._context) {
  o._isValid = false;
  o._display = nullptr;
  o._context = nullptr;
}

EGLContext& EGLContext::operator=(EGLContext&& o) {
  std::swap(_isValid, o._isValid);
  std::swap(_display, o._display);
  std::swap(_context, o._context);
  return *this;
}

EGLContext::EGLContext(::EGLDisplay display, ::EGLConfig config)
    : EGLContext(display, config, nullptr) {}

EGLContext::EGLContext(::EGLDisplay display,
                       ::EGLConfig config,
                       ::EGLContext share)
    : _isValid(false), _display(display), _context(nullptr) {
  EGLint attributes[] = {EGL_CONTEXT_CLIENT_VERSION, 2, EGL_NONE};

  _context = eglCreateContext(display, config, share, attributes);

  if (_context != nullptr) {
    _isValid = true;
  }
}

EGLContext::~EGLContext() {
  if (_context != nullptr) {
    EGLBoolean destroyed = eglDestroyContext(_display, _context);
    if (destroyed != EGL_TRUE) {
      RL_LOG_EGL_ERROR();
      RL_ASSERT(false);
    }
  }
}

bool EGLContext::isValid() const {
  return _isValid;
}

bool EGLContext::makeCurrent(const EGLSurface& surface) const {
  if (!surface.isValid()) {
    return false;
  }

  auto surfaceHandle = surface.handle();

  return eglMakeCurrent(_display, surfaceHandle, surfaceHandle, _context) ==
         EGL_TRUE;
}

bool EGLContext::clearCurrent() const {
  return eglMakeCurrent(_display, nullptr, nullptr, nullptr) == EGL_TRUE;
}

}  // namespace testrunner
}  // namespace rl
