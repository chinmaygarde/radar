// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef RADAR_GRAPHICSTESTRUNNER_EGLCONFIG_H_
#define RADAR_GRAPHICSTESTRUNNER_EGLCONFIG_H_

#include <Core/Macros.h>
#include "EGLSurface.h"

namespace rl {
namespace testrunner {

class EGLConnection;

class EGLConfig {
 public:
  struct BufferComponents {
    size_t red;
    size_t green;
    size_t blue;
    size_t alpha;
    size_t depth;
    size_t stencil;

    BufferComponents(size_t aRed,
                     size_t aGreen,
                     size_t aBlue,
                     size_t aAlpha,
                     size_t aDepth,
                     size_t aStencil);
  };

  EGLConfig();

  EGLConfig(EGLConfig&& config);

  EGLConfig& operator=(EGLConfig&&);

  ~EGLConfig();

  bool isValid() const;

 private:
  bool _isValid;
  ::EGLConfig _config;

  friend class EGLConnection;

  EGLConfig(::EGLDisplay display,
            EGLSurface::Type type,
            const BufferComponents& buffer);

  ::EGLConfig handle() const;

  RL_DISALLOW_COPY_AND_ASSIGN(EGLConfig);
};

}  // namespace testrunner
}  // namespace rl

#endif  // RADAR_GRAPHICSTESTRUNNER_EGLCONFIG_H_
