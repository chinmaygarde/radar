/*
 *  This source file is part of the Radar project.
 *  Licensed under the MIT License. See LICENSE file for details.
 */

#include "EGLConfig.h"

namespace rl {
namespace testrunner {

EGLConfig::BufferComponents::BufferComponents(size_t aRed,
                                              size_t aGreen,
                                              size_t aBlue,
                                              size_t aAlpha,
                                              size_t aDepth,
                                              size_t aStencil)
    : red(aRed),
      green(aGreen),
      blue(aBlue),
      alpha(aAlpha),
      depth(aDepth),
      stencil(aStencil) {}

EGLConfig::EGLConfig() : _isValid(false), _config(nullptr) {}

EGLConfig::EGLConfig(EGLConfig&& o) : _isValid(o._isValid), _config(o._config) {
  o._isValid = false;
  o._config = nullptr;
}

EGLConfig& EGLConfig::operator=(EGLConfig&&) = default;

EGLConfig::EGLConfig(::EGLDisplay display,
                     EGLSurface::Type type,
                     const BufferComponents& buffer)
    : _isValid(false), _config(nullptr) {
  EGLint attributes[] = {
      EGL_RENDERABLE_TYPE,
      EGL_OPENGL_ES2_BIT,
      EGL_CONFORMANT,
      EGL_OPENGL_ES2_BIT,
      EGL_SURFACE_TYPE,
      type == EGLSurface::Type::Window ? EGL_WINDOW_BIT : EGL_PBUFFER_BIT,
      EGL_RED_SIZE,
      static_cast<EGLint>(buffer.red),
      EGL_GREEN_SIZE,
      static_cast<EGLint>(buffer.green),
      EGL_BLUE_SIZE,
      static_cast<EGLint>(buffer.blue),
      EGL_ALPHA_SIZE,
      static_cast<EGLint>(buffer.alpha),
      EGL_DEPTH_SIZE,
      static_cast<EGLint>(buffer.depth),
      EGL_STENCIL_SIZE,
      static_cast<EGLint>(buffer.stencil),
      EGL_NONE,
  };

  EGLint count = 0;

  if (eglChooseConfig(display, attributes, &_config, 1, &count) != EGL_TRUE) {
    return;
  }

  if (_config != nullptr) {
    _isValid = true;
  }
}

EGLConfig::~EGLConfig() {}

bool EGLConfig::isValid() const {
  return _isValid;
}

::EGLConfig EGLConfig::handle() const {
  return _config;
}

}  // namespace testrunner
}  // namespace rl
