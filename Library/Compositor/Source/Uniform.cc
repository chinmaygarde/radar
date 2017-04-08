/*
 *  This source file is part of the Radar project.
 *  Licensed under the MIT License. See LICENSE file for details.
 */

#include "Uniform.h"

namespace rl {
namespace compositor {

void SetUniform(GLint index, const geom::Matrix& matrix) {
  GLMatrix floatMatrix = matrix;
  glUniformMatrix4fv(index, 1, GL_FALSE,
                     reinterpret_cast<const GLfloat*>(&floatMatrix));
}

void SetUniform(GLint index, const entity::Color& color) {
  glUniform4f(index, color.red, color.green, color.blue, color.alpha);
}

void SetUniform(GLint index, const entity::Color& color, double alpha) {
  glUniform4f(index, color.red, color.green, color.blue, color.alpha * alpha);
}

void SetUniform(GLint index, double alpha) {
  glUniform1f(index, alpha);
}

void SetUniform(GLint index, const geom::Size& size) {
  glUniform2f(index, size.width, size.height);
}

}  // namespace compositor
}  // namespace rl
