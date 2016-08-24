// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef RADAR_GRAPHICSTESTRUNNER_EGLUTILS_H_
#define RADAR_GRAPHICSTESTRUNNER_EGLUTILS_H_

#include <Core/Macros.h>

#include <string>

#define RL_LOG_EGL_ERROR() \
  RL_LOG("EGL Error: %s", rl::testrunner::EGLGetError().c_str())

namespace rl {
namespace testrunner {

std::string EGLGetError();

}  // namespace testrunner
}  // namespace rl

#endif  // RADAR_GRAPHICSTESTRUNNER_EGLUTILS_H_
