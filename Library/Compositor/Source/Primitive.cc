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

const double Primitive::AlphaThreshold = 1e-3;

Primitive::Primitive() : _opacity(1.0) {}

Primitive::~Primitive() = default;

Primitive::Primitive(Primitive&& primitive) = default;

void Primitive::setSize(geom::Size size) {
  _size = size;
}

void Primitive::setModelViewMatrix(geom::Matrix modelViewMatrix) {
  _modelViewMatrix = modelViewMatrix;
}

void Primitive::setSolidColor(entity::Color solidColor) {
  _solidColor = solidColor;
}

void Primitive::setOpacity(double opacity) {
  _opacity = opacity;
}

bool Primitive::render(Frame& frame) const {
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

  glUniform4f(program.contentColorUniform(), _solidColor.red, _solidColor.green,
              _solidColor.blue, _solidColor.alpha * _opacity);

  glUniform2f(program.sizeUniform(), _size.width, _size.height);

  /*
   *  Setup vertices and draw.
   */
  bool drawn = frame.context().unitBoxVertices().draw();

  RL_GLAssert("No errors while rendering");
  RL_ASSERT(drawn);

  return true;
}

}  // namespace compositor
}  // namespace rl

#endif  // !RL_OS_WINDOWS
