// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "Program/TextureProgram.h"

namespace rl {
namespace compositor {

static const char TextureVertexShader[] = R"--(

  attribute vec3 A_Position;

  uniform mat4 U_MVP;
  uniform vec2 U_Size;

  varying vec2 V_TextureCoordinates;

  void main() {
    V_TextureCoordinates = vec2(A_Position.x, A_Position.y);
    gl_Position = U_MVP * vec4(A_Position.x * U_Size.x,
                               A_Position.y * U_Size.y,
                               0.0, 1.0);
  }

  )--";

static const char TextureFragmentShader[] = R"--(

#ifdef GL_ES
  precision mediump float;
#endif

  uniform sampler2D U_Texture;

  varying vec2 V_TextureCoordinates;

  void main() {
    gl_FragColor = texture2D(U_Texture, V_TextureCoordinates);
  }
  
  )--";

TextureProgram::TextureProgram()
    : Program::Program(TextureVertexShader, TextureFragmentShader),
      _modelViewProjectionUniform(-1),
      _sizeUniform(-1),
      _positionAttribute(-1) {}

void TextureProgram::onLinkSuccess() {
  _modelViewProjectionUniform = indexForUniform("U_MVP");
  _sizeUniform = indexForUniform("U_Size");
  _positionAttribute = indexForAttribute("A_Position");
}

GLint TextureProgram::modelViewProjectionUniform() const {
  return _modelViewProjectionUniform;
}

GLint TextureProgram::sizeUniform() const {
  return _sizeUniform;
}

GLint TextureProgram::positionAttribute() const {
  return _positionAttribute;
}

}  // namespace compositor
}  // namespace rl
