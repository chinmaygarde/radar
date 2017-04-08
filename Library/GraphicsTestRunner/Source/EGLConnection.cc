/*
 *  This source file is part of the Radar project.
 *  Licensed under the MIT License. See LICENSE file for details.
 */

#include "EGLConnection.h"

namespace rl {
namespace testrunner {

EGLConnection::EGLConnection()
    : _isValid(false), _display(eglGetDisplay(EGL_DEFAULT_DISPLAY)) {
  if (_display == EGL_NO_DISPLAY) {
    return;
  }

  if (eglInitialize(_display, &_version.major, &_version.minor) == EGL_FALSE) {
    return;
  }

  _isValid = true;
}

EGLConnection::~EGLConnection() {
  if (_display != EGL_NO_DISPLAY) {
    EGLBoolean terminated = eglTerminate(_display);
    RL_ASSERT(terminated);
  }
}

EGLConnection::Version EGLConnection::version() const {
  return _version;
}

bool EGLConnection::isValid() const {
  return _isValid;
}

EGLConfig EGLConnection::createConfig(
    EGLSurface::Type type,
    const EGLConfig::BufferComponents& buffer) const {
  if (!_isValid) {
    return {};
  }

  return EGLConfig{_display, type, buffer};
}

EGLSurface EGLConnection::createSurface(EGLSurface::Type type,
                                        const EGLConfig& config,
                                        EGLSurface::EGLSize size) const {
  if (!_isValid) {
    return {};
  }

  return EGLSurface{type, _display, config.handle(), size};
}

EGLContext EGLConnection::createContext(const EGLConfig& config) const {
  if (!_isValid) {
    return {};
  }

  return EGLContext{_display, config.handle()};
}

}  // namespace testrunner
}  // namespace rl
