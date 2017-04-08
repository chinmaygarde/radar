/*
 *  This source file is part of the Radar project.
 *  Licensed under the MIT License. See LICENSE file for details.
 */

#include "Program/TextureProgram.h"

namespace rl {
namespace compositor {

static const char TextureVertexShader[] = R"--(

  attribute vec2 A_Position;

  uniform mat4 U_MVP;
  uniform vec2 U_Size;

  varying vec2 V_TextureCoordinates;

  void main() {
    V_TextureCoordinates = A_Position;
    gl_Position = U_MVP * vec4(A_Position * U_Size, 0.0, 1.0);
  }

  )--";

static const char TextureFragmentShader[] = R"--(

#ifdef GL_ES
  precision mediump float;
#endif

  uniform sampler2D U_Texture;
  uniform float U_Alpha;

  varying vec2 V_TextureCoordinates;

  void main() {
    gl_FragColor = U_Alpha * texture2D(U_Texture, V_TextureCoordinates);
  }
  
  )--";

TextureProgram::TextureProgram()
    : Program::Program(TextureVertexShader, TextureFragmentShader),
      _modelViewProjectionUniform(-1),
      _sizeUniform(-1),
      _alphaUniform(-1),
      _textureUniform(-1),
      _positionAttribute(-1) {}

void TextureProgram::onLinkSuccess() {
  _modelViewProjectionUniform = indexForUniform("U_MVP");
  _sizeUniform = indexForUniform("U_Size");
  _alphaUniform = indexForUniform("U_Alpha");
  _textureUniform = indexForUniform("U_Texture");
  _positionAttribute = indexForAttribute("A_Position");
}

GLint TextureProgram::modelViewProjectionUniform() const {
  return _modelViewProjectionUniform;
}

GLint TextureProgram::sizeUniform() const {
  return _sizeUniform;
}

GLint TextureProgram::alphaUniform() const {
  return _alphaUniform;
}

GLint TextureProgram::textureUniform() const {
  return _textureUniform;
}

GLint TextureProgram::positionAttribute() const {
  return _positionAttribute;
}

}  // namespace compositor
}  // namespace rl
