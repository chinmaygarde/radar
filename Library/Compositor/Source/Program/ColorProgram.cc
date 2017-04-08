/*
 *  This source file is part of the Radar project.
 *  Licensed under the MIT License. See LICENSE file for details.
 */

#include "ColorProgram.h"
#include <string>

namespace rl {
namespace compositor {

static const char ColorVertexShader[] = R"--(

  attribute vec2 A_Position;

  uniform mat4 U_MVP;
  uniform vec2 U_Size;

  void main() {
    gl_Position = U_MVP * vec4(A_Position * U_Size, 0.0, 1.0);
  }

)--";

static const char ColorFragmentShader[] = R"--(

#ifdef GL_ES
  precision mediump float;
#endif

  uniform vec4 U_ContentColor;

  void main() {
    gl_FragColor = U_ContentColor;
  }
  
)--";

ColorProgram::ColorProgram()
    : Program::Program(ColorVertexShader, ColorFragmentShader),
      _modelViewProjectionUniform(-1),
      _contentColorUniform(-1),
      _sizeUniform(-1),
      _positionAttribute(-1) {}

void ColorProgram::onLinkSuccess() {
  _modelViewProjectionUniform = indexForUniform("U_MVP");
  _contentColorUniform = indexForUniform("U_ContentColor");
  _sizeUniform = indexForUniform("U_Size");
  _positionAttribute = indexForAttribute("A_Position");
}

GLint ColorProgram::modelViewProjectionUniform() const {
  return _modelViewProjectionUniform;
}

GLint ColorProgram::contentColorUniform() const {
  return _contentColorUniform;
}

GLint ColorProgram::sizeUniform() const {
  return _sizeUniform;
}

GLint ColorProgram::positionAttribute() const {
  return _positionAttribute;
}

}  // namespace compositor
}  // namespace rl
