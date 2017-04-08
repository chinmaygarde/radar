/*
 *  This source file is part of the Radar project.
 *  Licensed under the MIT License. See LICENSE file for details.
 */

#pragma once

#include <Core/Macros.h>
#include <string>

#define RL_LOG_EGL_ERROR() \
  RL_LOG("EGL Error: %s", rl::testrunner::EGLGetError().c_str())

namespace rl {
namespace testrunner {

std::string EGLGetError();

}  // namespace testrunner
}  // namespace rl
