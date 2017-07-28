/*
 *  This source file is part of the Radar project.
 *  Licensed under the MIT License. See LICENSE file for details.
 */

#include "StrokeProgram.h"

namespace rl {
namespace compositor {

static constexpr char kStrokeVertexShader[] = R"--(

  attribute vec2 A_Position;

  uniform mat4 U_ModelViewMatrix;
  uniform mat4 U_ProjectionMatrix;
  uniform vec2 U_Size;

  void main() {
    gl_Position = U_ProjectionMatrix * U_ModelViewMatrix * vec4(A_Position * U_Size, 0.0, 1.0);
  }

  )--";

static constexpr char kStrokeFragmentShader[] = R"--(

#ifdef GL_ES
  precision mediump float;
#endif

  uniform vec4 U_ContentColor;

  void main() {
    gl_FragColor = U_ContentColor;
  }
  
  )--";

StrokeProgram::StrokeProgram()
    : Program(kStrokeVertexShader, kStrokeFragmentShader) {}

StrokeProgram::~StrokeProgram() = default;

void StrokeProgram::onLinkSuccess() {
  _modelViewUniform = indexForUniform("U_ModelViewMatrix");
  _projectionUniform = indexForUniform("U_ProjectionMatrix");
  _contentColorUniform = indexForUniform("U_ContentColor");
  _sizeUniform = indexForUniform("U_Size");
  _positionAttribute = indexForAttribute("A_Position");
}

GLint StrokeProgram::modelViewUniform() const {
  return _modelViewUniform;
}

GLint StrokeProgram::projectionUniform() const {
  return _projectionUniform;
}

GLint StrokeProgram::contentColorUniform() const {
  return _contentColorUniform;
}

GLint StrokeProgram::sizeUniform() const {
  return _sizeUniform;
}

GLint StrokeProgram::positionAttribute() const {
  return _positionAttribute;
}

}  // namespace compositor
}  // namespace rl
