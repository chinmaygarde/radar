/*
 *  This source file is part of the Radar project.
 *  Licensed under the MIT License. See LICENSE file for details.
 */

#include "StrokeProgram.h"

namespace rl {
namespace compositor {

static constexpr char kStrokeVertexShader[] = R"--(

  attribute vec2 A_Position;
  attribute vec2 A_Normal;

  uniform mat4 U_ModelViewProjectionMatrix;
  uniform vec2 U_Size;
  uniform float U_StrokeSize;

  varying vec2 V_Normal;

  void main() {
    vec2 strokeOffset = A_Normal * vec2(U_StrokeSize * 0.5);

    vec2 vertexPosition = (A_Position * U_Size) + strokeOffset;

    gl_Position =  U_ModelViewProjectionMatrix * vec4(vertexPosition, 0.0, 1.0);

    V_Normal = A_Normal;
  }

  )--";

static constexpr char kStrokeFragmentShader[] = R"--(

#ifdef GL_ES
  precision mediump float;
#endif

  uniform vec4 U_ContentColor;

  varying vec2 V_Normal;

  void main() {
    // TODO: V_Normal must be used for anti-aliasing.
    gl_FragColor = U_ContentColor;
  }
  
  )--";

StrokeProgram::StrokeProgram()
    : Program(kStrokeVertexShader, kStrokeFragmentShader) {}

StrokeProgram::~StrokeProgram() = default;

void StrokeProgram::onLinkSuccess() {
  _modelViewProjectionUniform = indexForUniform("U_ModelViewProjectionMatrix");
  _contentColorUniform = indexForUniform("U_ContentColor");
  _sizeUniform = indexForUniform("U_Size");
  _strokeSizeUniform = indexForUniform("U_StrokeSize");
  _positionAttribute = indexForAttribute("A_Position");
  _normalAttribute = indexForAttribute("A_Normal");
}

GLint StrokeProgram::modelViewProjectionUniform() const {
  return _modelViewProjectionUniform;
}

GLint StrokeProgram::contentColorUniform() const {
  return _contentColorUniform;
}

GLint StrokeProgram::sizeUniform() const {
  return _sizeUniform;
}

GLint StrokeProgram::strokeSizeUniform() const {
  return _strokeSizeUniform;
}

GLint StrokeProgram::positionAttribute() const {
  return _positionAttribute;
}

GLint StrokeProgram::normalAttribute() const {
  return _normalAttribute;
}

}  // namespace compositor
}  // namespace rl
