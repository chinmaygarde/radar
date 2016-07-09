// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <Compositor/Primitive/ColoredBoxPrimitive.h>
#include <Compositor/Frame.h>

#include "OpenGL.h"
#include "BoxVertices.h"
#include "ProgramCatalog.h"

namespace rl {
namespace compositor {

ColoredBoxPrimitive::ColoredBoxPrimitive() = default;

ColoredBoxPrimitive::~ColoredBoxPrimitive() = default;

void ColoredBoxPrimitive::setColor(entity::Color color) {
  _color = std::move(color);
}

void ColoredBoxPrimitive::prepareToRender(BackEndPass& backEndPass) {}

bool ColoredBoxPrimitive::render(Frame& frame) const {
  /*
   *  Select the program to use
   */
  auto& program = frame.context().programCatalog().boxProgram();

  if (!program.use()) {
    return false;
  }

  /*
   *  Setup Uniform Data
   */
  GLMatrix modelViewProjection = frame.projectionMatrix() * _modelViewMatrix;

  glUniformMatrix4fv(program.modelViewProjectionUniform(), 1, GL_FALSE,
                     reinterpret_cast<const GLfloat*>(&modelViewProjection));

  glUniform4f(program.contentColorUniform(),  //
              _color.red,                     //
              _color.green,                   //
              _color.blue,                    //
              _color.alpha * _opacity);

  glUniform2f(program.sizeUniform(), _size.width, _size.height);

  /*
   *  Setup vertices and draw.
   */
  bool drawn = frame.context().unitBoxVertices().draw();

  RL_GLAssert("No errors while rendering");

  return drawn;
}

}  // namespace compositor
}  // namespace rl
