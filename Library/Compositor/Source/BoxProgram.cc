// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "BoxProgram.h"

#include <string>

namespace rl {
namespace compositor {

static const std::string BoxVertexShader = R"--(

  attribute vec3 position;

  uniform mat4 U_MVP;
  uniform vec2 U_Size;

  void main() {
    gl_Position = U_MVP * vec4(position.x * U_Size.x,
                               position.y * U_Size.y,
                               0.0, 1.0);
  }

)--";

static const std::string BoxFragmentShader = R"--(

#ifdef GL_ES
  precision mediump float;
#endif

  uniform vec4 U_ContentColor;

  void main() {
    gl_FragColor = U_ContentColor;
  }
  
)--";

BoxProgram::BoxProgram()
    : Program::Program({"U_MVP", "U_Size", "U_ContentColor"},
                       BoxVertexShader,
                       BoxFragmentShader),
      _modelViewProjectionUniform(-1),
      _contentColorUniform(-1),
      _sizeUniform(-1) {}

void BoxProgram::onLinkSuccess() {
  _modelViewProjectionUniform = indexForUniform("U_MVP");
  _contentColorUniform = indexForUniform("U_ContentColor");
  _sizeUniform = indexForUniform("U_Size");
}

unsigned int BoxProgram::modelViewProjectionUniform() const {
  return _modelViewProjectionUniform;
}

unsigned int BoxProgram::contentColorUniform() const {
  return _contentColorUniform;
}

unsigned int BoxProgram::sizeUniform() const {
  return _sizeUniform;
}

}  // namespace compositor
}  // namespace rl
