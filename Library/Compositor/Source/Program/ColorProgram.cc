// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "ColorProgram.h"

#include <string>

namespace rl {
namespace compositor {

static const char ColorVertexShader[] = R"--(

  attribute vec3 position;

  uniform mat4 U_MVP;
  uniform vec2 U_Size;

  void main() {
    gl_Position = U_MVP * vec4(position.x * U_Size.x,
                               position.y * U_Size.y,
                               0.0, 1.0);
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
    : Program::Program({"U_MVP", "U_Size", "U_ContentColor"},
                       ColorVertexShader,
                       ColorFragmentShader),
      _modelViewProjectionUniform(-1),
      _contentColorUniform(-1),
      _sizeUniform(-1) {}

void ColorProgram::onLinkSuccess() {
  _modelViewProjectionUniform = indexForUniform("U_MVP");
  _contentColorUniform = indexForUniform("U_ContentColor");
  _sizeUniform = indexForUniform("U_Size");
}

unsigned int ColorProgram::modelViewProjectionUniform() const {
  return _modelViewProjectionUniform;
}

unsigned int ColorProgram::contentColorUniform() const {
  return _contentColorUniform;
}

unsigned int ColorProgram::sizeUniform() const {
  return _sizeUniform;
}

}  // namespace compositor
}  // namespace rl
