/*
 *  This source file is part of the Radar project.
 *  Licensed under the MIT License. See LICENSE file for details.
 */

#pragma once

#include <Core/Macros.h>
#include <EGL/egl.h>

namespace rl {
namespace testrunner {

class EGLConnection;
class EGLConfig;
class EGLContext;

class EGLSurface {
 public:
  enum class Type {
    Window,
    PBuffer,
  };

  struct EGLSize {
    EGLint width;
    EGLint height;

    EGLSize(EGLint aWidth, EGLint aHeight) : width(aWidth), height(aHeight) {}
  };

  EGLSurface();

  EGLSurface(EGLSurface&& surface);

  EGLSurface& operator=(EGLSurface&&);

  ~EGLSurface();

  bool isValid() const;

 private:
  bool _isValid;
  ::EGLDisplay _display;
  ::EGLSurface _surface;

  friend class EGLConnection;
  friend class EGLConfig;
  friend class EGLContext;

  EGLSurface(Type type, ::EGLDisplay display, ::EGLConfig config, EGLSize size);

  ::EGLSurface handle() const;

  RL_DISALLOW_COPY_AND_ASSIGN(EGLSurface);
};

}  // namespace testrunner
}  // namespace rl
