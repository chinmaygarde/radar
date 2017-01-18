// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef RADAR_GRAPHICSTESTRUNNER_EGLCONNECTION_H_
#define RADAR_GRAPHICSTESTRUNNER_EGLCONNECTION_H_

#include <Core/Macros.h>
#include <EGL/egl.h>
#include "EGLConfig.h"
#include "EGLContext.h"

namespace rl {
namespace testrunner {

class EGLConnection {
 public:
  struct Version {
    EGLint major;
    EGLint minor;

    Version() : major(0), minor(0) {}
  };

  EGLConnection();

  ~EGLConnection();

  Version version() const;

  bool isValid() const;

  EGLConfig createConfig(EGLSurface::Type type,
                         const EGLConfig::BufferComponents& buffer) const;

  EGLSurface createSurface(EGLSurface::Type type,
                           const EGLConfig& config,
                           EGLSurface::EGLSize size) const;

  EGLContext createContext(const EGLConfig& config) const;

 private:
  bool _isValid;
  Version _version;
  ::EGLDisplay _display;

  RL_DISALLOW_COPY_AND_ASSIGN(EGLConnection);
};

}  // namespace testrunner
}  // namespace rl

#endif  // RADAR_GRAPHICSTESTRUNNER_EGLCONNECTION_H_
