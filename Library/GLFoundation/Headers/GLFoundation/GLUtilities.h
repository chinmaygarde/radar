/*
 *  This source file is part of the Radar project.
 *  Licensed under the MIT License. See LICENSE file for details.
 */

#pragma once

#define RL_GLAssert(x, ...) \
  rl::gl::AssertError(__FILE__, __LINE__, (x), ##__VA_ARGS__);

namespace rl {
namespace gl {

bool IsFramebufferComplete(void);

void AssertError(const char* file, int line, const char* fmt...);

void DescribeFramebuffer(void);

}  // namespace gl
}  // namespace rl
