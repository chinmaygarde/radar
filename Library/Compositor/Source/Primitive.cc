// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <Core/Config.h>

#if !RL_OS_WINDOWS

#include <Compositor/Primitive.h>
#include <Compositor/Context.h>

#include "OpenGL.h"
#include "BoxVertices.h"
#include "ProgramCatalog.h"

namespace rl {
namespace compositor {

Primitive::Primitive() : _color(entity::ColorWhiteTransparent), _opacity(1.0) {}

const entity::Color& Primitive::color() const {
  return _color;
}

void Primitive::setColor(const entity::Color& color) {
  _color = color;
}

bool Primitive::render(Frame& frame,
                       const geom::Matrix& modelViewMatrix,
                       const geom::Size& size) {
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
  GLMatrix modelViewProjection = frame.projectionMatrix() * modelViewMatrix;

  glUniformMatrix4fv(program.modelViewProjectionUniform(), 1, GL_FALSE,
                     reinterpret_cast<const GLfloat*>(&modelViewProjection));

  glUniform4f(program.contentColorUniform(), _color.red, _color.green,
              _color.blue, _color.alpha * _opacity);

  glUniform2f(program.sizeUniform(), size.width, size.height);

  /*
   *  Setup vertices and draw.
   */
  bool drawn = frame.context().unitBoxVertices().draw();

  RL_GLAssert("No errors while rendering");
  RL_ASSERT(drawn);

  return true;
}

double Primitive::opacity() const {
  return _opacity;
}

void Primitive::setOpacity(double opacity) {
  _opacity = opacity;
}

}  // namespace compositor
}  // namespace rl

#endif  // !RL_OS_WINDOWS
