/*
 *  This source file is part of the Radar project.
 *  Licensed under the MIT License. See LICENSE file for details.
 */

#include "ConsoleRendererProgram.h"

namespace rl {
namespace compositor {

static constexpr char kConsoleRendererVertexShader[] = R"--(
  uniform mat4 U_ProjectionMatrix;

  attribute vec2 A_Position;
  attribute vec2 A_UV;
  attribute vec4 A_Color;

  varying vec2 V_UV;
  varying vec4 V_Color;

  void main() {
    V_UV = A_UV;
    V_Color = A_Color;
    gl_Position = U_ProjectionMatrix * vec4(A_Position.xy, 0, 1);
  }
  )--";

static constexpr char kConsoleRendererFragmentShader[] = R"--(

#ifdef GL_ES
  precision mediump float;
#endif

  uniform sampler2D U_Texture;

  varying vec2 V_UV;
  varying vec4 V_Color;

  void main() {
    gl_FragColor = V_Color * texture2D(U_Texture, V_UV.st);
  }
  )--";

ConsoleRendererProgram::ConsoleRendererProgram()
    : Program(kConsoleRendererVertexShader, kConsoleRendererFragmentShader) {}

ConsoleRendererProgram::~ConsoleRendererProgram() = default;

void ConsoleRendererProgram::onLinkSuccess() {
  _textureUniform = indexForUniform("U_Texture");
  _projectionMatrixUniform = indexForUniform("U_ProjectionMatrix");
  _positionAttribute = indexForAttribute("A_Position");
  _uvAttribute = indexForAttribute("A_UV");
  _colorAttribute = indexForAttribute("A_Color");
}

GLint ConsoleRendererProgram::textureUniform() const {
  return _textureUniform;
}

GLint ConsoleRendererProgram::projectionMatrixUniform() const {
  return _projectionMatrixUniform;
}

GLint ConsoleRendererProgram::positionAttribute() const {
  return _positionAttribute;
}

GLint ConsoleRendererProgram::uvAttribute() const {
  return _uvAttribute;
}

GLint ConsoleRendererProgram::colorAttribute() const {
  return _colorAttribute;
}

}  // namespace compositor
}  // namespace rl
