// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <Compositor/Primitive.h>

#define RL_OPENGL_ALLOWED 1
#include <Compositor/OpenGL.h>

namespace rl {

Primitive::Primitive() : _contentColor(ColorWhiteTransparent), _opacity(1.0) {
}

const Color& Primitive::contentColor() const {
  return _contentColor;
}

void Primitive::setContentColor(const Color& color) {
  _contentColor = color;
}

void Primitive::render(Frame& frame,
                       const Matrix& modelViewMatrix,
                       const Size& size) {
  /*
   *  Select the program to use
   */
  auto program = static_cast<const BasicPrimitiveProgram*>(
      frame.programCatalog()->useProgramType(
          ProgramCatalog::Type::BasicPrimitve));

  // clang-format off
  GLCoord coords[] = {
    { 0.0, 1.0, 0.0 },
    { 0.0, 0.0, 0.0 },
    { 1.0, 1.0, 0.0 },
    { 1.0, 0.0, 0.0 },
  };
  // clang-format on

  /*
   *  Upload Vertex Data.
   */
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, coords);

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

  glDrawArrays(GL_TRIANGLE_STRIP, 0, sizeof(coords) / sizeof(GLCoord));

  RL_GLAssert("No errors while rendering");
}

double Primitive::opacity() const {
  return _opacity;
}

void Primitive::setOpacity(double opacity) {
  _opacity = opacity;
}

}  // namespace rl
