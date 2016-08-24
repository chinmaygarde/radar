// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "EGLSurface.h"

#include "EGLUtils.h"

namespace rl {
namespace testrunner {

EGLSurface::EGLSurface()
    : _isValid(false), _display(nullptr), _surface(nullptr) {}

EGLSurface::EGLSurface(EGLSurface&& o)
    : _isValid(o._isValid), _display(o._display), _surface(o._surface) {
  o._isValid = false;
  o._display = nullptr;
  o._surface = nullptr;
}

EGLSurface& EGLSurface::operator=(EGLSurface&& o) {
  std::swap(_isValid, o._isValid);
  std::swap(_display, o._display);
  std::swap(_surface, o._surface);
  return *this;
}

EGLSurface::EGLSurface(Type type,
                       ::EGLDisplay display,
                       ::EGLConfig config,
                       EGLSize size)
    : _isValid(false), _display(display), _surface(nullptr) {
  if (type != Type::PBuffer) {
    RL_LOG("Explicitly sized surfaces can only be pbuffers.");
    return;
  }

  const EGLint attrib[] = {
      EGL_WIDTH, size.width, EGL_HEIGHT, size.height, EGL_NONE,
  };

  _surface = eglCreatePbufferSurface(display, config, attrib);

  if (_surface != nullptr) {
    _isValid = true;
  }
}

EGLSurface::~EGLSurface() {
  if (_surface != nullptr) {
    EGLBoolean destroyed = eglDestroySurface(_display, _surface);
    if (destroyed != EGL_TRUE) {
      RL_LOG_EGL_ERROR();
      RL_ASSERT(false);
    }
  }
}

bool EGLSurface::isValid() const {
  return _isValid;
}

::EGLSurface EGLSurface::handle() const {
  return _surface;
}

}  // namespace testrunner
}  // namespace rl
