// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "Uniform.h"

namespace rl {
namespace compositor {

void SetUniform(GLint index, const geom::Matrix& matrix) {
  GLMatrix floatMatrix = matrix;
  ::glUniformMatrix4fv(index, 1, GL_FALSE,
                       reinterpret_cast<const GLfloat*>(&floatMatrix));
}

void SetUniform(GLint index, const entity::Color& color) {
  ::glUniform4f(index, color.red, color.green, color.blue, color.alpha);
}

void SetUniform(GLint index, const entity::Color& color, double alpha) {
  ::glUniform4f(index, color.red, color.green, color.blue, color.alpha * alpha);
}

void SetUniform(GLint index, double alpha) {
  ::glUniform1f(index, alpha);
}

void SetUniform(GLint index, const geom::Size& size) {
  ::glUniform2f(index, size.width, size.height);
}

}  // namespace compositor
}  // namespace rl
