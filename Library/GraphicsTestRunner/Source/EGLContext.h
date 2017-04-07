// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#pragma once

#include <Core/Macros.h>
#include <EGL/egl.h>
#include "EGLSurface.h"

namespace rl {
namespace testrunner {

class EGLConnection;

class EGLContext {
 public:
  EGLContext();

  EGLContext(EGLContext&& context);

  EGLContext& operator=(EGLContext&&);

  ~EGLContext();

  bool isValid() const;

  bool makeCurrent(const EGLSurface& surface) const;

  bool clearCurrent() const;

 private:
  bool _isValid;
  ::EGLDisplay _display;
  ::EGLContext _context;

  friend class EGLConnection;

  EGLContext(::EGLDisplay display, ::EGLConfig config);

  EGLContext(::EGLDisplay display, ::EGLConfig config, ::EGLContext share);

  RL_DISALLOW_COPY_AND_ASSIGN(EGLContext);
};

}  // namespace testrunner
}  // namespace rl
