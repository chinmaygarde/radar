// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <Core/Config.h>

#if !RL_OS_WINDOWS

#include <Compositor/Primitive.h>

#include "OpenGL.h"
#include "BoxVertices.h"

namespace rl {
namespace compositor {

Primitive::Primitive()
    : _contentColor(entity::ColorWhiteTransparent), _opacity(1.0) {}

const entity::Color& Primitive::contentColor() const {
  return _contentColor;
}

void Primitive::setContentColor(const entity::Color& color) {
  _contentColor = color;
}

bool Primitive::render(Frame& frame,
                       const geom::Matrix& modelViewMatrix,
                       const geom::Size& size) {
  /*
   *  Select the program to use
   */
  auto program = static_cast<const BasicPrimitiveProgram*>(
      frame.programCatalog()->useProgramType(
          ProgramCatalog::Type::BasicPrimitve));

  /*
   *  Setup Uniform Data
   */
  GLMatrix modelViewProjection = frame.projectionMatrix() * modelViewMatrix;

  glUniformMatrix4fv(program->modelViewProjectionUniform(), 1, GL_FALSE,
                     reinterpret_cast<const GLfloat*>(&modelViewProjection));

  glUniform4f(program->contentColorUniform(), _contentColor.red,
              _contentColor.green, _contentColor.blue,
              _contentColor.alpha * _opacity);

  glUniform2f(program->sizeUniform(), size.width, size.height);

  /*
   *  Setup vertices and draw.
   */
  bool drawn = frame.unitBoxVertices().draw();

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
